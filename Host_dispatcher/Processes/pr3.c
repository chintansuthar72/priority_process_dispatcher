#include <stdio.h>      
#include <stdlib.h>  
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC

void main()  
{     
    // use rand() function to generate the number  
    double time_spent = 0.0;
 
    clock_t begin = clock();

    printf("Testing process 3\n") ;

    printf (" The random number is: %d", rand());  
    printf ("\n The random number is: %d", rand());  
    printf (" \n The random number is: %d", rand());  
    printf ("\n The random number is: %d", rand());  
    
    clock_t end = clock();
 
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
    printf("\n\nThe elapsed time is %f seconds", time_spent);
}  
