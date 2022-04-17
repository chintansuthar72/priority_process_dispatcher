#include <stdio.h>
#include "Process_control_block.h"
#include <signal.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    PcbPtr dispatcher_queue = NULL;
    PcbPtr Feedback_queues[3] = {NULL, NULL, NULL}; // 0-higher priority
    fill_dispatcher_list(&dispatcher_queue);
    int dispathcer_time = 0;
    int currently_running = 0;
    PcbPtr current_process = NULL;
    while (dispatcher_queue || Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2] || currently_running)
    {
        while (dispatcher_queue && dispatcher_queue->arrival_time <= dispathcer_time)
        {
            PcbPtr temp = dequeuePcb(&dispatcher_queue);
            temp->priority = 2;
            Feedback_queues[2] = enqueuePcb(Feedback_queues[2], temp);
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
            else if (Feedback_queues[0] || Feedback_queues[1] || Feedback_queues[2])
            {
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
                printf("From feedback queue 1\n");
            }
            else if (Feedback_queues[1])
            {
                current_process = dequeuePcb(&Feedback_queues[1]);
                printf("From feedback queue 2\n");
            }
            else if (Feedback_queues[0])
            {
                current_process = dequeuePcb(&Feedback_queues[0]);
                printf("From feedback queue 3\n");
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
            printf("second %d , running : %s\n", dispathcer_time, current_process->args[0]);
            dispathcer_time++;
            sleep(1);
        }
    }

    return 0;
}