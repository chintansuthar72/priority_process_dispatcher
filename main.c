#include<stdio.h>
#include<Process_control_block.h>
#include<signal.h>

int main(int argc, const char *argv[])
{
    PcbPtr process_queue_head=NULL,current_process=NULL;
    int currently_running_any_process = 0;  // 0 if not, 1 if anyone process is running

    int dispatcher_timer = 0;

    while(currently_running_any_process || (process_queue_head != NULL)) {
        if(currently_running_any_process) { // any process is running
            if(current_process->remaining_time == 0){
                // send SIGINT signal
                if(kill(current_process->pid,SIGINT))
                    fprintf(stderr,"terminate of %d failed",(int)current_process->pid);
                else
                    printf("Process %s finished executing\n",current_process->args[0]);
                // Free up memory
                currently_running_any_process = 0;
            } else { // Process is still running
                printf("Currently running process : %s\n",current_process->args[0]);
                current_process->remaining_time--; // decrease remainingcputime
            }
        } else { // No process is running but process queue is not empty
            if(dispatcher_timer >= process_queue_head->arrival_time) {
                current_process = dequeuePcb(process_queue_head);
                // Fork and exec to run process
                int process_id = fork();
                if(process_id == 0) { // In the child process
                    currently_running_any_process = 1;
                    execvp(current_process->args[0], current_process->args);
                } else { // In the parent process
                    // Do nothing
                }
            }
        }

        sleep(1);
        ++dispatcher_timer;
    }
    return 0;
}
