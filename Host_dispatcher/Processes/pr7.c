#include<stdio.h>
#include<stdlib.h>
int main()
{
	long long int n = 1000000000;
    long long int sum = 0;
	printf("Testing Process 7\n") ;
	for(int i=0;i<=n;i++)
	{
        sum += i;
	}
    printf("Sum from 0 to %lld calculated\n",n);
	return 0;
}
