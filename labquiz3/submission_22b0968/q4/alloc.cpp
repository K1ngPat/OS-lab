#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <map>

#include "alloc.h"

using namespace std;
#define NUMSLOTS MEMSIZE/MINALLOC
char *memstart = NULL;
int bitmap[NUMSLOTS];

int init_alloc(){
    memstart = (char *)malloc(MEMSIZE);
    for(int i=0; i<NUMSLOTS; i++)
    {
        bitmap[i] = 0;
    }
    if(memstart == NULL)
    {
        return -1;
    }
    return 0;
}

int cleanup(){

    for(int i=0; i<NUMSLOTS; i++)
    {
        bitmap[i] = 0;
    }
    free((void *)memstart);
    memstart = NULL;
    return 0;


}

char *alloc(int size){

    if(size % MINALLOC != 0)
    {
        return (char *)0;
    }
    int reqslots = size/MINALLOC;
    char *ggs = (char *)0;
    for(int i = 0; i < NUMSLOTS; i += reqslots)
    {
        bool flag = true;
        for(int j = 0; j < reqslots; j++)
        {
            if(bitmap[i + j] != 0)
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            
            for(int j = 0; j < reqslots; j++)
            {
                bitmap[i + j] = reqslots;
            }

            ggs = memstart + (i * MINALLOC);
            
            break;
        }
    }

    return ggs;

}

void dealloc(char* f_mem){

    
    int indx = ((f_mem - memstart) & 0xfff) / MINALLOC;
    int slots_to_clean = bitmap[indx];

    for(int j = 0; j < slots_to_clean; j++)
    {
        bitmap[indx + j] = 0;
    }


}
