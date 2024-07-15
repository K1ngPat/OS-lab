#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    int f = fork();

    if (f == 0)
    {
        char *ptr = shm_attach(8);
        sleep(50);
        printf(1, "Value: %d\n", *(uint *)ptr);
        exit();
    }
    else
    {
        char *ptr = shm_attach(69);
        if (ptr == 0)
        {
            printf(1, "Error: shm_attach failed\n");
        }
        else
        {
            printf(1, "***testcase failed. award 0.\n");
            exit();
        }
        ptr = shm_attach(8);
        *(uint *)ptr = 17;
        wait();
    }
    exit();
}