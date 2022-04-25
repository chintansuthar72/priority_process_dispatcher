#include <stdio.h>
int main(int argc, char*argv[], char*env[])
{

	char **ptr;
	printf("Testing Process 5\n") ;
	// Printing all Environmental variables
	for(ptr=env; *ptr != 0; ptr++)
		printf("%s\n",*ptr);
	
}