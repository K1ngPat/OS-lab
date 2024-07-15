#include "types.h"
#include "stat.h"
#include "user.h"

int
main()
{
    int f = fork();

    if (f == 0)
    {
        char *ptr = shm_attach(8);
        sleep(50);
        printf(1, "Value: %d\n", *(uint *)ptr);
        shm_detach(ptr);
        exit();
    }
    else
    {
        char *ptr = shm_attach(8);
        *(uint *)ptr = 17;
        shm_detach(ptr);
        wait();
    }
    exit();
}