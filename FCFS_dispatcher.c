#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

const int MAXARGS = 10;

struct pcb
{
    pid_t pid;                  // process id
    char *args[MAXARGS];        // program name and arguments
    int arrivaltime;
    int remainingcputime;
    struct pcb *next;
};

struct pcb *process_queue_head = NULL, *process_queue_tail = NULL;
// Fill this queue from text file or something else

void fill_process_queue() {

}

int main() {
    fill_process_queue();
    struct pcb *current_process = NULL;

    int currently_running_any_process = 0; 
    // 0 if not, 1 if anyone process is running

    int dispatcher_timer = 0;

    while(currently_running_any_process || (process_queue_head != NULL && process_queue_tail != NULL)) {
        if(currently_running_any_process) { // any process is running
            if(current_process->remainingcputime == 0){
                // send SIGINT signal
                if(kill(current_process->pid,SIGINT))
                    fprintf(stderr,"terminate of %d failed",(int)current_process->pid);
                // Free up memory
            } else { // Process is still running
                // decrease remainingcputime
            }
        } else { // No process is running but process queue is not empty
            if(dispatcher_timer >= process_queue_head->arrivaltime) {
                current_process = process_queue_head;
                process_queue_head = process_queue_head->next;
                // Fork and exec to run process
                int process_id = fork();
                if(fork() == 0) { // In the child process
                    execvp(current_process->args[0], current_process->args);
                } else { // In the parent process

                }
            }
        }
        sleep(1);
        ++dispatcher_timer;
    }
    exit(0);
}