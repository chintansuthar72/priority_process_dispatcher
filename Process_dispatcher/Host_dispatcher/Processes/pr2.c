#include<stdio.h>
#include<unistd.h>
int main(){
	int n = 100 ;
	// printf("%d\n",getpid());
	sleep(5);
	printf("\nTesting Process 2\n") ;
	for(int i=0;i<n;i++) printf("%d,",i); 
	}
