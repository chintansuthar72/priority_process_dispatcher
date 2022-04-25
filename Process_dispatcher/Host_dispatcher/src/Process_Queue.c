#include "../include/Process_control_block.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

// function to start a process
void start_pcb(PcbPtr process)
{
    pipe(process->File_descriptors);
    process->status = RUNNING; // set status to running
    process->pid = fork();
    if (process->pid == 0) // in child
    {
        // redirect output to file descriptor
        close(process->File_descriptors[READ]);
        dup2(process->File_descriptors[WRITE], 1);

        execv(process->args[0], process->args);

        printf("Cannot start process : %s\n", process->args[0]);
        close(process->File_descriptors[WRITE]); // close output
        exit(-1);
    }
    else
    {
        close(process->File_descriptors[WRITE]);
    }
}

PcbPtr Stop_Pcb(PcbPtr process)
{
    if (kill(process->pid, SIGSTOP)) // send SIGSTOP signal
    {
        Terminate_Pcb(process); // terminate if singal process can't be paused
        return NULL;
    }
    process->status = SUSPENDED;
    return process;
}

// check whether process is running
int is_running(PcbPtr process)
{
    char state;
    char filepath[100];
    sprintf(filepath, "/proc/%d/stat", process->pid);
    FILE *fproc = fopen(filepath, "r");

    //  no file means no process
    if (fproc == NULL)
        return 0;

    // scan the flag from file
    fscanf(fproc, "%*s %*[^)]) %c", &state);
    fclose(fproc);

    // R - for running, S - for sleeping
    if (state == 'R' || state == 'S')
        return 1;
    else
        return 0;
}

PcbPtr Resume_Pcb(PcbPtr process)
{

    if (kill(process->pid, SIGCONT)) // send SIGCONT signal
        return NULL;
    process->status = RUNNING;
    return process;
}

PcbPtr Create_Pcb()
{
    // initialzie a new Pcb
    PcbPtr temp = (PcbPtr)malloc(sizeof(PCB));
    temp->status = CREATED;
    temp->time_elapsed = 0.0;
    return temp;
}

PcbPtr enqueuePcb(PcbPtr head, PcbPtr process) // returns new head
{
    if (head == NULL) // if head is null, make the process as head
    {
        head = process;
        return head;
    }
    // else find the tail and add process to it
    PcbPtr temp = head;
    while (temp != NULL)
    {
        if (temp->next == NULL)
            break;
        temp = temp->next;
    }
    temp->next = process;
    temp = NULL;
    return head;
}

PcbPtr dequeuePcb(PcbPtr *head) // returns dequeued process
{
    if (head == NULL)
        return NULL;
    PcbPtr temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    return temp;
}

// fill list from the given input file
void fill_dispatcher_list(PcbPtr *head, char file[])
{
    FILE *fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("Invalid input file provided!! \n");
        exit(-1);
    }

    float arrtime;
    int num_args;
    char name[100];
    PcbPtr proc;

    while (fscanf(fp, "%f %d", &arrtime, &num_args) != EOF)
    {
        proc = Create_Pcb();
        proc->arrival_time = arrtime;   
        for (int i = 0; i < num_args; i++)
        {
            proc->args[i] = (char *)malloc(100 * sizeof(char));
            fscanf(fp, "%s", name);
            strcpy(proc->args[i], name);
        }
        *head = enqueuePcb(*head, proc);
    }
    fclose(fp);
}

PcbPtr Terminate_Pcb(PcbPtr process)
{
    if (kill(process->pid, SIGKILL))
    {
        return NULL;
    }
    process->status = TERMINATED;
    return process;
}

int queue_size(PcbPtr head) // returns size of queue
{
    PcbPtr temp = head;
    int size = 0;
    while (temp)
    {   
            size++;
        temp = temp->next;
    }
    temp = NULL;
    return size;
}