#include <stdio.h>
#include <stdlib.h>  
int main (){
    FILE *fp = fopen("../Processes/pr1.c","r");
    // Return if could not open file
    if (fp == NULL)
      return 0;
 	printf("\nTesting Process 1\n") ;
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
    return(0);
}