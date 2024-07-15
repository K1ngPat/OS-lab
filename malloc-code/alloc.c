#include "alloc.h"

struct meminterval
{
    int start;  // inclusive
    int end; // exclusive
};

int contains(struct meminterval *m, int offs)
{
    if(offs >= m->end || offs < m->start)
    {
        return 0;
    }

    return 1;
}

struct manager
{
    void *pages[1];
    struct meminterval chunks[PAGESIZE/MINALLOC];
};

struct manager mang;

int init_alloc()
{
    void *k = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (k == MAP_FAILED)
    {
        return -1;
    }
    mang.pages[0] = k;

    for(int i=0; i<PAGESIZE/MINALLOC; i++)
    {
        mang.chunks[i].end = -1;
        mang.chunks[i].start = -1;
    }
    return 0;
}

int cleanup()
{
    return munmap(mang.pages[0], PAGESIZE);
}

char *alloc(int a)
{
    if (a % 8 != 0)
    {
        return NULL;
    }
    
    int cck = -1;
    int brk = 0;
    for(int offs=0; offs < PAGESIZE; offs += MINALLOC)
    {
        int ind = 1;
        for(int i=0; i<PAGESIZE/MINALLOC; i++)
        {
            
            if(contains(&mang.chunks[i], offs))
            {
                ind = 0;
                break;
            }
            
        }
        if(ind)
        {
            brk++;
        }
        if (brk * MINALLOC >= a)
        {
            cck = offs - (brk-1) * MINALLOC;

            for(int i=0; i<PAGESIZE/MINALLOC; i++)
            {
                
                if(mang.chunks[i].start == -1 && mang.chunks[i].end == -1)
                {
                    mang.chunks[i].start = cck;
                    mang.chunks[i].end = cck + a;
                    break;
                }
                
            }

            break;
        }
        
    }

    char *ret = (char *)((long)mang.pages[0] + (long)cck);
    return ret;
}

void dealloc(char * g)
{
    for(int i=0; i<PAGESIZE/MINALLOC; i++)
    {
        
        if((long)mang.chunks[i].start + (long)mang.pages[0] == (long)g)
        {
            mang.chunks[i].start = -1;
            mang.chunks[i].end = -1;
            break;
        }

        
    }
}