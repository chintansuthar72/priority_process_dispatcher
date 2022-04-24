#include <unistd.h>
#include <sys/types.h>
#define MAXARGS 10 // max number of arguments for process
#define CREATED 0
#define RUNNING 1
#define SUSPENDED 2
#define TERMINATED 3
#define Time_Interval 0.1
// typedef to use short names
typedef struct Process_control_block PCB;
typedef PCB *PcbPtr;

struct Process_control_block // structure definition for a process
{
    pid_t pid;           // process id
    int priority;
    char *args[MAXARGS]; // list of arguments
    float arrival_time;
    int status;
    int File_descriptors[2];
    struct Process_control_block *next; // link to next process
};

void start_pcb(PcbPtr process);
// PcbPtr Terminate_Pcb(PcbPtr process);
PcbPtr Create_Pcb();
PcbPtr enqueuePcb(PcbPtr head, PcbPtr process);
PcbPtr dequeuePcb(PcbPtr *head);
int queue_size(PcbPtr head);
int is_running(PcbPtr process);
void fill_dispatcher_list(PcbPtr *head);
PcbPtr Stop_Pcb(PcbPtr process);
PcbPtr Resume_Pcb(PcbPtr process);
