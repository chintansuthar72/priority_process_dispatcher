#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/Process_control_block.h" // header file

/*-------------------- Priority Process Dispatcher -------------------------------
------------------Source code for Feed back dispatcher  -----------------------------*/

sem_t thread_sem; // semaphore used to avoid jumbled output

void *print_output(void *args); // thread function pointer to display output

void Sigchild_Handler() // handler for SIGCHLD signal
{
    // executed when child processes is paused or terminated
    pid_t pid;
    int status;

    // Tracing all signals
    // WNOHANG : returns the status of available signals immediately without waiting
    // Helps to avoiding child getting zombified!!!
    while ((pid == waitpid(-1, &status, WNOHANG)) != 0)
    {
        if (pid == 0)
            break; // pid=0 means no signal is remaining, so break
    }
}

// function to get time
uint64_t get_gtod_clock_time()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0)
        return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
    else
        return 0;
}

// function to get time difference
double get_timer(uint64_t start_time)
{
    uint64_t end_time = get_gtod_clock_time();
    double time_diff = (end_time - start_time) / 1000000.0;
    return time_diff;
}

int main(int argc, char *argv[])
{

    uint64_t start_time, endtime;
    start_time = get_gtod_clock_time();
    // file name should be passed from command line
    if (argc < 2)
    {
        perror("No input file provided!!\n");
        exit(-1);
    }

    /* Process queues:
       1) dispatcher_queue which is loaded will processes from text file
       2) Feed_back_queues :
            A three level feed back queue systems where queue with index 2
            has higher priority and queue with index 0 has lower priority
            =>Processe are loaded into index 2 queue will be moved to next
                level queue in each time_interval
            => When only queue with index 0 has processes remaining then
                it behaves as Round_Robin_dispatcher   */
    PcbPtr dispatcher_queue = NULL;
    PcbPtr Feedback_queues[3] = {NULL, NULL, NULL};

    fill_dispatcher_list(&dispatcher_queue, argv[1]); // initialize dispatcher queue

    FILE *flog = fopen("log.txt", "w"); // for logging errors and details

    sem_init(&thread_sem, 0, 1);       // Initialize semaphore
    signal(SIGCHLD, Sigchild_Handler); // set handler to SIGCHLD

    // structure to be used with nanosleep function
    struct timespec request, remaining;
    request.tv_nsec = Time_Interval * 1000000000;
    request.tv_sec = request.tv_nsec / 100000000;
    request.tv_nsec %= 100000000;

    int num_processes = queue_size(dispatcher_queue);
    pthread_t threads[num_processes]; // array of threads to shown output of each process
    int thread_index = 0;

    // Timer for Dispatcher
    uint64_t dispatcher_time = get_gtod_clock_time();
    uint64_t previous_time;
    int currently_running = 0;     // flag to check if any process is running
    PcbPtr current_process = NULL; // pointer to currently running process
    // while any of queues is not empty or any process is running
    while (dispatcher_queue || Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2] || currently_running)
    {

        // insert all processes from dispatcher queue to feed back queue which should be started by this time
        while (dispatcher_queue && dispatcher_queue->arrival_time <= get_timer(dispatcher_time))
        {
            PcbPtr temp = dequeuePcb(&dispatcher_queue);

            // Assigning highest priority for the first time to all processes
            // and inserting into highest priority queue
            temp->priority = 2;
            Feedback_queues[2] = enqueuePcb(Feedback_queues[2], temp);
        }

        // If any process is running
        if (currently_running)
        {
            current_process->time_elapsed += get_timer(previous_time); // increase time
            previous_time = get_gtod_clock_time();

            // check if process took too long!!!
            if (current_process->time_elapsed >= Max_Time_Limit)
            {
                fprintf(flog, "\n%f - Process : %s took too long(%fs)\n", get_timer(dispatcher_time), current_process->args[0], current_process->time_elapsed);
                Terminate_Pcb(current_process);
                fprintf(flog, "%f - Terminating process %s!!\n", get_timer(dispatcher_time), current_process->args[0]);
                current_process = NULL;
                currently_running = 0;
            }
            else
            {
                // check if current process finished execution
                if (is_running(current_process) == 0)
                {
                    // if finished execution, then create a thread and display its output
                    currently_running = 0; // set flag

                    // pointer to be passed for thread
                    PcbPtr *fd = (PcbPtr *)malloc(sizeof(PcbPtr));
                    *fd = current_process;
                    pthread_create(&threads[thread_index++], NULL, print_output, (void *)fd);

                    fprintf(flog, "\n%f - process : %s finished executing \n", get_timer(dispatcher_time), current_process->args[0]);
                    fprintf(flog, "\tstarted at t=%f , Finished", current_process->start_time);
                    fprintf(flog, " at t= %fs, executed time= %fs--\n", get_timer(dispatcher_time), current_process->time_elapsed);
                }

                // if curent process is still running, pause it and push into next level queue
                else if (Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2])
                {
                    if (Stop_Pcb(current_process) == NULL)
                    {
                        fprintf(flog, "\n%f - Process : %s can't be paused\n", get_timer(dispatcher_time), current_process->args[0]);
                    }
                    else
                        fprintf(flog, "\n%f - Process : %s is paused\n", get_timer(dispatcher_time), current_process->args[0]);
                    current_process->priority--; // reduce priority
                    int queue_pos = current_process->priority;
                    if (queue_pos < 0) // 0 is lowest possible priority
                        queue_pos = 0;

                    // insert process into appropriate queue
                    Feedback_queues[queue_pos] = enqueuePcb(Feedback_queues[queue_pos], current_process);

                    // set flags
                    current_process = NULL;
                    currently_running = 0;
                }
            }
        }

        // If no process is running and all feed back queues are not empty
        else if (Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2])
        {
            // Take process from highest priority queue
            if (Feedback_queues[2])
            {
                current_process = dequeuePcb(&Feedback_queues[2]);
            }
            else if (Feedback_queues[1])
            {
                current_process = dequeuePcb(&Feedback_queues[1]);
            }
            else if (Feedback_queues[0])
            {
                current_process = dequeuePcb(&Feedback_queues[0]);
            }

            // Resume or Start the processes depending on its status
            if (current_process->status == SUSPENDED)
            {
                if (Resume_Pcb(current_process) == NULL)
                {
                    fprintf(flog, "Can't resume process : %s\n", current_process->args[0]);
                    currently_running = 0;
                }
                else
                {
                    fprintf(flog, "\n-Process %s is resumed\n", current_process->args[0]);
                    currently_running = 1;
                }
                previous_time = get_gtod_clock_time();
            }
            else if (current_process->status == CREATED)
            {
                start_pcb(current_process);
                current_process->start_time = get_timer(dispatcher_time);
                fprintf(flog, "\n%f - started process : %s\n", current_process->start_time, current_process->args[0]);
                currently_running = 1;
                previous_time = get_gtod_clock_time();
            }
        }

        // sleep for Time_Interval
        while (nanosleep(&request, &remaining) == -1)
        {
            request.tv_sec = remaining.tv_sec;
            request.tv_nsec = remaining.tv_nsec;
        };
    }
    endtime = get_gtod_clock_time();
    uint64_t time_diff = endtime - start_time;

    // close file
    fclose(flog);

    for (int i = 0; i < thread_index; i++) // wait for all threads to finish
        pthread_join(threads[i], NULL);

    printf("\n\n- All processes ended!!!");
    printf("\nTime taken = %f\n", time_diff / 1000000.0);
    return 0;
}

void *print_output(void *args)
{
    // extract pcb from arguemnt passed
    PcbPtr *ptr = args;
    PcbPtr curr = *ptr;
    int fd = curr->File_descriptors[READ]; // file descriptor to read output

    // critical section, using semaphore to avoid jumbled output
    sem_wait(&thread_sem);

    // Read any output written by this process into pipe and display it
    int bytes_read, buffer_size = 100;
    char buff[buffer_size + 1];
    bytes_read = read(fd, buff, buffer_size);
    if (bytes_read != 0 && bytes_read != -1)
    {
        buff[bytes_read] = '\0';
        printf("\n\033[0;32m ----- Output from %s: -----\033[0;37m\n%s", curr->args[0], buff);
    }
    while (bytes_read == buffer_size)
    {
        bytes_read = read(fd, buff, buffer_size);
        if (bytes_read == 0 || bytes_read == -1)
            break;
        buff[bytes_read] = '\0';
        printf("%s", buff);
    }
    sem_post(&thread_sem); // unlock semaphore

    close(curr->File_descriptors[READ]); // close file descriptor
}
