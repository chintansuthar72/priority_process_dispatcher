#include <stdio.h>
#include "../include/Process_control_block.h"
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#define READ 0
#define WRITE 1
sem_t thread_sem;
void *print_output(void *args)
{
    PcbPtr *ptr = args;
    PcbPtr curr = *ptr;
    int fd = curr->File_descriptors[READ];
    sem_wait(&thread_sem);
    int bytes_read, buffer_size = 10;
    char buff[buffer_size + 1];
    bytes_read = read(fd, buff, buffer_size);
    if (bytes_read != 0 && bytes_read != -1)
    {
        buff[bytes_read] = '\0';
        printf("Output from %s:\n", curr->args[0]);
    }
    while (bytes_read == buffer_size)
    {
        bytes_read = read(fd, buff, buffer_size);
        if (bytes_read == 0 || bytes_read == -1)
            break;
        buff[bytes_read] = '\0';
        printf("%s", buff);
    }
    sem_post(&thread_sem);
    close(curr->File_descriptors[READ]);
}
void wait_handler()
{
    printf("waiting here\n");
    wait(NULL);
    printf("done here\n");
}
int main()
{
    // sem_init(&thread_sem, 0, 1);
    signal(SIGCHLD, wait_handler);
    PcbPtr dispatcher_queue = NULL;
    PcbPtr Feedback_queues[3] = {NULL, NULL, NULL}; // 2-higher priority
    fill_dispatcher_list(&dispatcher_queue);
    int num_processes = queue_size(dispatcher_queue);
    pthread_t threads[num_processes];
    int thread_index = 0;
    float dispathcer_time = 0;
    int currently_running = 0;
    PcbPtr current_process = NULL;
    while (dispatcher_queue || Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2] || currently_running)
    {
        printf("dispatcher time: %f, ",dispathcer_time);
        while (dispatcher_queue && dispatcher_queue->arrival_time <= dispathcer_time)
        {
            PcbPtr temp = dequeuePcb(&dispatcher_queue);
            temp->priority = 2;
            Feedback_queues[2] = enqueuePcb(Feedback_queues[2], temp);
        }
        if (currently_running)
        {
                printf("is running returen\n");
            if (is_running(current_process) == 0)
            {
                printf("is running returen\n");
                currently_running = 0;
                printf("%s finished executing\n", current_process->args[0]);
                PcbPtr *fd = (PcbPtr *)malloc(sizeof(PcbPtr));
                *fd = current_process;
                pthread_create(&threads[thread_index++], NULL, print_output, (void *)fd);
                printf("process : %s finished executing\n", current_process->args[0]);
                currently_running = 0;
            }
            else if (Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2])
            {
                printf("is returend 0\n");
                Stop_Pcb(current_process);
                printf("process : %s was paused\n", current_process->args[0]);
                current_process->priority--;
                int queue_pos = current_process->priority;
                if (queue_pos < 0)
                    queue_pos = 0;
                Feedback_queues[queue_pos] = enqueuePcb(Feedback_queues[queue_pos], current_process);
                current_process = NULL;
                currently_running = 0;
            }
        }
        else if (Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2])
        {
            if (Feedback_queues[2])
            {
                current_process = dequeuePcb(&Feedback_queues[2]);
                // printf("From feedback queue 1\n");
            }
            else if (Feedback_queues[1])
            {
                current_process = dequeuePcb(&Feedback_queues[1]);
                // printf("From feedback queue 2\n");
            }
            else if (Feedback_queues[0])
            {
                current_process = dequeuePcb(&Feedback_queues[0]);
                // printf("From feedback queue 3\n");
            }
            if (current_process->status == SUSPENDED)
            {
                Resume_Pcb(current_process);
                currently_running = 1;
            }
            else if (current_process->status == CREATED)
            {
                start_pcb(current_process);
                currently_running = 1;
            }
        }
        if (currently_running)
        {
            printf("second %f , running : %s\n", dispathcer_time , current_process->args[0]);
        }
            dispathcer_time += Time_Interval;
            sleep(Time_Interval);
    }
    printf("reached end\n");
    for (int i = 0; i < num_processes; i++)
        pthread_join(threads[i], NULL);
    sem_destroy(&thread_sem);
    return 0;
}