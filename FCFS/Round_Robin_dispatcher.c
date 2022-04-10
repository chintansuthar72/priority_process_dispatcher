#include <stdio.h>
#include "Process_control_block.h"
#include <signal.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    PcbPtr dispathcer_queue = NULL, Round_robin_queue = NULL;
    fill_dispatcher_list(&dispathcer_queue);
    int dispathcer_time = 0;
    int currently_running = 0;
    PcbPtr current_process = NULL;
    while (dispathcer_queue || Round_robin_queue || currently_running)
    {
        while (dispathcer_queue && dispathcer_queue->arrival_time <= dispathcer_time)
        {
            PcbPtr temp = dequeuePcb(&dispathcer_queue);
            Round_robin_queue = enqueuePcb(Round_robin_queue, temp);
        }
         if (currently_running)
        {
            current_process->remaining_time--;
            if (current_process->remaining_time == 0)
            {
                Terminate_Pcb(current_process);
                printf("process : %s finished executing\n", current_process->args[0]);
                currently_running = 0;
            }
            else if (Round_robin_queue)
            {
                Stop_Pcb(current_process);
                printf("process : %s was paused\n", current_process->args[0]);
                Round_robin_queue = enqueuePcb(Round_robin_queue, current_process);
                current_process = NULL;
                currently_running = 0;
             }
        }
        else if (Round_robin_queue)
        {
             current_process = dequeuePcb(&Round_robin_queue);
            if (current_process->status == SUSPENDED)
            {
                Resume_Pcb(current_process);
                currently_running=1;
            }
            else if (current_process->status == CREATED)
            {
                start_pcb(current_process);
                currently_running = 1;
            }
        }
        if (currently_running)
            printf("currently running : %s\n", current_process->args[0]);
        dispathcer_time++;
        sleep(1);
    }

    return 0;
}