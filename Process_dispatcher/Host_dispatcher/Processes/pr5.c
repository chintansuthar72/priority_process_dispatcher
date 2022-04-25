#include <stdio.h>
int main(int argc, char*argv[], char*env[])
{
	char **ptr;
	printf("\n\nTesting ENV VAR\n") ; 
	int counter = 0;
	for(ptr=env; *ptr != 0 ; ptr++)
	{
		printf("%s\n",*ptr);
		counter++ ;
		if(counter >5)
			break ;
	}
	printf("\nThese were some of the environment variables\n\n") ;
	
} 	