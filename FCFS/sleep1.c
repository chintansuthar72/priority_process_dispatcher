#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("I am from sleep 1\n");
    sleep(1);
    pause();
}