#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{

	// make two process which run same
	// program after this instruction
	pid_t a = fork();
    int stats = -999;
    pid_t b = waitpid(-1, &stats, 0);
    
    if(a==-1)
    {
        printf("Could not fork\n");
    }
    else if (a==0)
    {
        printf("I am child\n");
    }
    else
    {
        printf("I am parent\n");
        if(a != b)
        {
            printf("Something fishy! %d %d\n", a, b);
        }
        else
        {
            printf("All good %d %d\n", a, b);
        }
    }


    return 0;
}
