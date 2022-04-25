#include <unistd.h>
#include <sys/types.h>
#define MAXARGS 10 // max number of arguments for process
// Below four micros are defined for defining process states
#define CREATED 0
#define RUNNING 1
#define SUSPENDED 2
#define TERMINATED 3

// Time interval (In one round, particular process will executed for "Time_Interval" time)
#define Time_Interval 0.001  // 1ms time interval

// typedef to use short names
typedef struct Process_control_block PCB;
typedef PCB *PcbPtr;

struct Process_control_block // structure definition for a process
{
    pid_t pid;           // process id
    int priority;       // Priority of queues - e.g 0,1,2
    char *args[MAXARGS]; // list of arguments
    float arrival_time;
    int status;         // CREATED, RUNNING, SUSPENDED, TERMINATED
    int File_descriptors[2];        // For pipe use
    struct Process_control_block *next; // link to next process
};

void start_pcb(PcbPtr process); // To start porcess for execution
// PcbPtr Terminate_Pcb(PcbPtr process);
PcbPtr Create_Pcb(); 
PcbPtr enqueuePcb(PcbPtr head, PcbPtr process); // To add process into process queue
PcbPtr dequeuePcb(PcbPtr *head); // To remove process from process queue
int queue_size(PcbPtr head); // Compute queue size
int is_running(PcbPtr process); // Check wheather process is running or not
void fill_dispatcher_list(PcbPtr *head); // Fill process queue from text file of processes
PcbPtr Stop_Pcb(PcbPtr process); // To suspend process
PcbPtr Resume_Pcb(PcbPtr process); // To resume suspended process

