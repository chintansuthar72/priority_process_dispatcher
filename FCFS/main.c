#include <stdio.h>
#include "Process_control_block.h"
#include <signal.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    PcbPtr process_queue_head = NULL, current_process = NULL;
    int currently_running_any_process = 0;
    // 0 if not, 1 if anyone process is running
    fill_dispatcher_list(&process_queue_head);
    int dispatcher_timer = 0;

    while (currently_running_any_process || (process_queue_head != NULL))
    {
        if (currently_running_any_process)
        { // any process is running
            if (current_process->remaining_time == 0)
            {
                if (Terminate_Pcb(current_process) == NULL)
                {
                    printf("Process %d could not be terminated\n", current_process->pid);
                }
                else
                {
                    printf("Process %s finished executing\n", current_process->args[0]);
                }
                currently_running_any_process = 0;
                // Free up memory
            }
            else
            { // Process is still running
                //     printf("Currently running process : %s\n", current_process->args[0]);
                current_process->remaining_time--;
                // decrease remainingcputime
            }
        }
        else
        { // No process is running but process queue is not empty
            if (dispatcher_timer >= process_queue_head->arrival_time)
            {
                current_process = dequeuePcb(&process_queue_head);
                start_pcb(current_process);
                printf("process %s started executing\n", current_process->args[0]);
                currently_running_any_process = 1;
                // start_pcb(current_process);
            }
        }

        sleep(1);
        ++dispatcher_timer;
    }
    return 0;
}
