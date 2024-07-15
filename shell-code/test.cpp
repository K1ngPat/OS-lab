#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	int *tester = (int *)malloc(sizeof(int) * ((long long int)1 << 31));
	printf("tester: %p\n", tester);
	for(int i = 0; i < ((long long int)1 << 31); i++)
	{
		tester[i] = i;
		// printf("%d ", tester[i]);
	}

	return 0;
}
