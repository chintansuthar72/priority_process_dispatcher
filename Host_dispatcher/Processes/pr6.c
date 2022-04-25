#include<stdio.h>
#include<stdlib.h>
// Factorial count function
int factorial(int n) {
    if( n == 1 || n == 2) {
        return n;
    }
    return n * factorial(n-1);
}
int main()
{
	int n = 50 ;
	printf("Testing Process 6\n") ;
    factorial(n);
    printf("Factorial of %d computed\n",n) ;
	return 0;
}
