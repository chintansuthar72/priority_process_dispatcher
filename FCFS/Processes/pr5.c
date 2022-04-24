#include <stdio.h>
int main(int argc, char*argv[], char*env[])
{

	char **ptr;
	printf("Testing pr5\n") ;

	for(ptr=env; *ptr != 0; ptr++)
		printf("%s\n",*ptr);
	
}