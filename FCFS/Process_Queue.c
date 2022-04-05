#include "Process_control_block.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
// function to start a process
void start_pcb(PcbPtr process)
{
    int child = fork();
    process->pid = child;
    if (child == 0)
    {
        execv(process->args[0], process->args);
        printf("Cannot start process : %s\n", process->args[0]);
    }
}

PcbPtr Terminate_Pcb(PcbPtr process)
{
    if (kill(process->pid, SIGINT))
        return NULL;
    return process;
}

PcbPtr Create_Pcb()
{
    PcbPtr temp = (PcbPtr)malloc(sizeof(PCB));
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