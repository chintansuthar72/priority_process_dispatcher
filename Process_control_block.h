#include <unistd.h>
#include <sys/types.h>
#define MAXARGS 10 // max number of arguments for process

struct Process_control_block // structure definition for a process
{
    pid_t pid;            // process id
    char *args[MAXARGS]; // list of arguments
    int arrival_time;
    int remaining_time;
    struct Process_control_block *next; // link to next process
};

// typedef to use short names
typedef struct Process_control_block PCB;
typedef PCB *PcbPtr;

PcbPtr start_pcb(PcbPtr process);
PcbPtr Terminate_Pcb(PcbPtr process);
PcbPtr Create_Pcb();
PcbPtr enqueuePcb(PcbPtr head, PcbPtr process) ;
PcbPtr dequeuePcb(PcbPtr head);
int queue_size(PcbPtr head);
