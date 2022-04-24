#include <stdio.h>
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC
#include <stdlib.h>  
 
int main ()
{
    // open the file
	double time_spent = 0.0;
 
    clock_t begin = clock();

    FILE *fp = fopen("./pr1.c","r");
 
    // Return if could not open file
    if (fp == NULL)
      return 0;
 
 	printf("Testing Process 2\n") ;
    do
    {
        // Taking input single character at a time
        char c = fgetc(fp);
 
        // Checking for end of file
        if (feof(fp))
            break ;
 
        printf("%c", c);
    }  while(1);

 	fclose(fp);

 	clock_t end = clock();
 
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
    printf("\n\nThe elapsed time is %f seconds", time_spent);
    

    
    return(0);
}