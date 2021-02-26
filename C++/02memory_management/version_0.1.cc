//testGdb.c
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;

#define HEAPSIZE 500
#define malloc_addr heap
#define malloc_size HEAPSIZE
#define mcb_size sizeof(mem_control_block)

static unsigned char has_initialized;
static intptr_t managed_memory_start;
static intptr_t managed_memory_end;
static intptr_t managed_memory_size;
unsigned char heap[HEAPSIZE];

typedef struct
{
    unsigned int is_available;
    unsigned int prior_blocksize;
    unsigned int current_blocksize;
} mem_control_block;

void my_malloc_init()
{
    mem_control_block *mcb = NULL;
    //the type of managed_memory_start must be long, not unsigned int.
    //cast from "unsigned char*" to "unsigned int" lost precision
    //              8 bytes            4 bytes
    managed_memory_start = (intptr_t)malloc_addr;
    managed_memory_size = malloc_size;
    managed_memory_end = managed_memory_start + managed_memory_size;

    mcb = (mem_control_block *)managed_memory_start;
    mcb->is_available = 1;
    mcb->prior_blocksize = 0;
    mcb->current_blocksize = managed_memory_size - mcb_size;
    has_initialized = 1;
    cout << "size of mem_control_block is " << mcb_size << endl;
    cout << "mcb->current_blocksize = " << mcb->current_blocksize << endl;
}

void *my_malloc(unsigned int numbytes)
{
    intptr_t current_location, otherbck_location;
    mem_control_block *current_location_mcb = NULL, *otherbck_loaction_mcb = NULL;
    void *memory_location = NULL;
    unsigned int process_blocksize;

    if (!has_initialized)
        my_malloc_init();

    current_location = managed_memory_start;
    while (current_location != managed_memory_end)
    {
        current_location_mcb = (mem_control_block *)current_location;
        if (current_location_mcb->is_available)
        {
            if (current_location_mcb->current_blocksize == numbytes)
            {
                current_location_mcb->is_available = 0;
                memory_location = (void *)(current_location + mcb_size);
                break;
            }
            else if (current_location_mcb->current_blocksize >= numbytes + mcb_size)
            {
                current_location_mcb->is_available = 0;
                process_blocksize = current_location_mcb->current_blocksize;
                current_location_mcb->current_blocksize = numbytes;

                //依次处理当前mcb的各个数据成员
                otherbck_loaction_mcb = (mem_control_block *)(current_location + numbytes + mcb_size);
                otherbck_loaction_mcb->is_available = 1;
                otherbck_loaction_mcb->prior_blocksize = numbytes;
                otherbck_loaction_mcb->current_blocksize = process_blocksize - numbytes - mcb_size;

                otherbck_location = current_location + process_blocksize + mcb_size;
                if (otherbck_location != managed_memory_end)
                {
                    otherbck_loaction_mcb = (mem_control_block *)otherbck_location;
                    otherbck_loaction_mcb->prior_blocksize = process_blocksize - numbytes - mcb_size;
                }
                memory_location = (void *)(current_location + mcb_size);
                break;
            }
        }
        current_location += current_location_mcb->current_blocksize + mcb_size;
    }
    return memory_location;
}

void my_free(void *firstbyte)
{
    intptr_t current_location, otherbck_location;
    mem_control_block *current_mcb = NULL, *next_mcb = NULL, *prior_mcb = NULL, *other_mcb = NULL;
    current_location = (intptr_t)firstbyte - mcb_size;
    current_mcb = (mem_control_block *)current_location;
    current_mcb->is_available = 1;
    //内存整理方式一：先前向整理，再后向整理
    if (current_location != managed_memory_start)
    {
        otherbck_location = current_location - current_mcb->prior_blocksize - mcb_size;
        prior_mcb = (mem_control_block *)otherbck_location;
        if (prior_mcb->is_available)
        {
            prior_mcb->current_blocksize = prior_mcb->current_blocksize + current_mcb->current_blocksize + mcb_size;
            current_location = otherbck_location;
            current_mcb = prior_mcb;
        }
    }

    otherbck_location = current_location + mcb_size + current_mcb->current_blocksize;
    if (otherbck_location != managed_memory_end)
    {
        next_mcb = (mem_control_block *)otherbck_location;
        if (next_mcb->is_available)
        {
            current_mcb->current_blocksize = current_mcb->current_blocksize + mcb_size + next_mcb->current_blocksize;
        }
    }

    otherbck_location = current_location + mcb_size + current_mcb->current_blocksize;
    if (otherbck_location != managed_memory_end)
    {
        other_mcb = (mem_control_block *)otherbck_location;
        other_mcb->prior_blocksize = current_mcb->current_blocksize;
    }

    /*
    //内存整理方式二：先后向整理，再前向整理
    otherbck_location = current_location + mcb_size + current_mcb->current_blocksize;
    if (otherbck_location != managed_memory_end) //这里可以改进
    {
        next_mcb = (mem_control_block *)otherbck_location;
        if (next_mcb->is_available)
        {
            current_mcb->current_blocksize += (mcb_size + next_mcb->current_blocksize);

            otherbck_location = current_location + current_mcb->current_blocksize + mcb_size;
            if (otherbck_location != managed_memory_end)
            {
                other_mcb = (mem_control_block *)otherbck_location;
                other_mcb->prior_blocksize = current_mcb->current_blocksize;
            }
        }
    }

    if (current_location != managed_memory_start)
    {
        prior_mcb = (mem_control_block *)(current_location - mcb_size - current_mcb->prior_blocksize);
        if (prior_mcb->is_available)
        {
            prior_mcb->current_blocksize += (mcb_size + current_mcb->current_blocksize);

            otherbck_location = current_location + mcb_size + current_mcb->current_blocksize;

            if (otherbck_location != managed_memory_end)
            {
                other_mcb = (mem_control_block *)otherbck_location;
                other_mcb->prior_blocksize = prior_mcb->current_blocksize;
            }
        }
    }*/
}

void print_memory()
{
    if (!has_initialized)
        my_malloc_init();

    intptr_t current_location = managed_memory_start;
    mem_control_block *current_location_mcb = NULL;

    cout << "The memory layout is :" << endl;
    while (current_location != managed_memory_end)
    {
        current_location_mcb = (mem_control_block *)current_location;
        cout << "addr: " << current_location << " | available " << current_location_mcb->is_available << " | prior_blocksize " << current_location_mcb->prior_blocksize << " | current_blocksize " << current_location_mcb->current_blocksize << endl;
        current_location = current_location + mcb_size + current_location_mcb->current_blocksize;
    }
}

int main(void)
{
    int *p1, *p2, *p3;

    memset((void *)heap, 1, HEAPSIZE);
    my_malloc_init();
    print_memory();

    p1 = (int *)my_malloc(sizeof(int));
    p2 = (int *)my_malloc(sizeof(int));
    p3 = (int *)my_malloc(sizeof(int));
    print_memory();

    my_free((void *)p2);
    p2 = NULL;
    my_free((void *)p1);
    my_free((void *)p3);
    print_memory();

    return 0;
}
