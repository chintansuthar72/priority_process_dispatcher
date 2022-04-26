#include<stdio.h>
#include<stdlib.h>
int fact(int n)
{
	int ans = 1 ;
	for(int i=1 ; i<=n ; i++)
	{
		ans = ans * i ;
	}
	return (ans) ;
}
int main(int argc,char *argv[])
{
	if(argc<2) 
	{
		printf("no argument provided\n");
		return 0;
	}
	int num=atoi(argv[1]);
	printf("\nTesting Factorial Function -_-\n") ;
	printf("Factorial(%d) = %d\n\n",num,fact(num)) ;
}