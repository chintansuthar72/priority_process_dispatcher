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
#define READ 0
#define WRITE 1
// function to start a process
void start_pcb(PcbPtr process)
{
    pipe(process->File_descriptors);
    process->status = RUNNING;
    process->pid = fork();
    if (process->pid == 0)
    {
        close(process->File_descriptors[READ]); // Close read end at child
        dup2(process->File_descriptors[WRITE],1); // Direct output at stdout
        execv(process->args[0], process->args);
        printf("Cannot start process : %s\n", process->args[0]);
        exit(-1);
    }
    else
    {
        close(process->File_descriptors[WRITE]); // Close write end at parent
    }
}

PcbPtr Stop_Pcb(PcbPtr process)
{
    if (kill(process->pid, SIGSTOP))
        return NULL;
    process->status = SUSPENDED;
    return process;
}
int is_running(PcbPtr process)
{
    kill(process->pid, 0); // Check process is running or not
    if (errno == ESRCH || errno==EPERM)
    {
        return 0;
    }
    return 1;
}

PcbPtr Resume_Pcb(PcbPtr process)
{

    if (kill(process->pid, SIGCONT))
        return NULL;
    process->status = RUNNING;
    return process;
}

PcbPtr Create_Pcb()
{
    PcbPtr temp = (PcbPtr)malloc(sizeof(PCB));
    temp->status = CREATED;
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

PcbPtr dequeuePcb(PcbPtr *head) // returns process dequeued
{
    if (head == NULL)
        return NULL;
    PcbPtr temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    return temp;
}
void fill_dispatcher_list(PcbPtr *head)
{
    FILE *fp = fopen("process_list.txt", "r");
    float arrtime;
    int num_args;
    char name[100];
    PcbPtr proc;
    while (fscanf(fp, "%f %d", &arrtime, &num_args) != EOF)
    {
        proc = Create_Pcb();
        proc->arrival_time = arrtime;
        int i = 0;
        for (int i = 0; i < num_args; i++)
        {
            proc->args[i] = (char *)malloc(100 * sizeof(char));
            fscanf(fp, "%s", name);
            strcpy(proc->args[i], name);
            *head = enqueuePcb(*head, proc);
        }
    }
}

int queue_size(PcbPtr head)
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
