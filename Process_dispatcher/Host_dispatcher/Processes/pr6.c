#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC


void ls()
{

    struct dirent *de; // Pointer for directory entry

	// opendir() returns a pointer of DIR type.
	printf("\nTesting LS command .-.-.-\n") ;
    char directory[100] = "." ;
    	DIR *dr = opendir(directory);

	if (dr == NULL) // opendir returns NULL if directory not opened
	{
		printf("Could not open current directory" );
		return ;
	}

	while ((de = readdir(dr)) != NULL)
			printf("%s\n", de->d_name);

	closedir(dr);
	printf("\n\n") ;
}


int main()
{

	ls() ;
}
