#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h> 
int main()
{
    int n = 10;
    struct timespec ts;
    ts.tv_nsec=4*100000000;
    ts.tv_sec=0.4;
    while (n--)
    {
        printf("hi\n");
        nanosleep(&ts,&ts);
    }
}
