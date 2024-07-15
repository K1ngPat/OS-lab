#include "types.h"
#include "stat.h"
#include "user.h"

int
main()
{
    int f = fork();

    if(f == 0){
        char* ptr1 = shm_attach(6);
        char* ptr2 = shm_attach(9);
        sleep(50);
        printf(1, "Value at ptr1: %d\n", *(uint*)ptr1);
        printf(1, "Value at ptr2: %d\n", *(uint*)ptr2);
        shm_detach(ptr1);
        shm_detach(ptr2);
        exit();
    }
    else{
        char* ptr1 = shm_attach(6);
        char* ptr2 = shm_attach(9);
        *(uint*)ptr1 = 17;
        *(uint*)ptr2 = 18;
        shm_detach(ptr1);
        shm_detach(ptr2);
        wait();
    }
    exit();
}