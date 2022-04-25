#include <unistd.h>
#include <sys/types.h>
#define MAXARGS 10 // max number of arguments for process
// flags
#define CREATED 0
#define RUNNING 1
#define SUSPENDED 2
#define TERMINATED 3
#define READ 0
#define WRITE 1
#define Time_Interval 0.001 // time interval
#define Max_Time_Limit 3.00   // upper bound for time taken by process
// typedef to use short names
typedef struct Process_control_block PCB;
typedef PCB *PcbPtr;

struct Process_control_block // structure definition for a process
{
    pid_t pid;           // process id
    int priority;        // priority
    char *args[MAXARGS]; // list of arguments
    float arrival_time;  // when this process should be started
    float start_time;
    float time_elapsed;                 // time taken by this process ( intial value =0 )
    int status;                         // status of process
    int File_descriptors[2];            // file descriptors used for capturing output using pipes
    struct Process_control_block *next; // link to next process
};

// functions
void start_pcb(PcbPtr process);
PcbPtr Terminate_Pcb(PcbPtr process);
PcbPtr Create_Pcb();
PcbPtr enqueuePcb(PcbPtr head, PcbPtr process);
PcbPtr dequeuePcb(PcbPtr *head);
int queue_size(PcbPtr head);
int is_running(PcbPtr process);
void fill_dispatcher_list(PcbPtr *head, char file[]);
PcbPtr Stop_Pcb(PcbPtr process);
PcbPtr Resume_Pcb(PcbPtr process);
