#include "timer_heap.h"

time_heap::time_heap(int cap) throw() : array(vector<heap_timer *>(cap + 1, NULL)), valid_size(0)
{
}

time_heap::time_heap(vector<heap_timer *> init_array) throw() : array(init_array), valid_size(array.size() - 1)
{
    for (int i = 1; i <= valid_size; i++)
        array[i]->index = i;
    for (int i = valid_size / 2; i >= 1; i--)
    {
        down_adjust(i, valid_size);
    }
}

time_heap::~time_heap()
{
    array.clear();
    valid_size = 0;
}

void time_heap::add_timer(heap_timer *timer)
{
    if (!timer)
        return;
    valid_size++;
    while (valid_size >= array.size())
        array.push_back(NULL);

    array[valid_size] = timer;
    timer->index = valid_size;
    up_adjust(1, valid_size);
    printf("add timer success, valid_size:%d, timer:%p, index:%d, array[%d]:%p, timeout:%ld\n", valid_size, timer, timer->index, timer->index, array[timer->index], timer->expire);
}
void time_heap::del_timer(heap_timer *timer)
{
    if (!timer || valid_size < 1)
        return;

    int indx = timer->index;
    if (indx > valid_size)
    {
        printf("error! delete timer index:%d > valid_size:%d\n", indx, valid_size);
        return;
    }
    delete array[indx];
    array[indx] = array[valid_size];
    array[valid_size--] = NULL;
    down_adjust(indx, valid_size);
}
heap_timer *time_heap::top_timer()
{
    if (!valid_size)
        return NULL;
    return array[1];
}
void time_heap::pop_timer()
{
    if (!valid_size)
        return;
    if (array[1])
    {
        heap_timer *ret = array[1];
        delete array[1];
        array[1] = NULL;
        array[1] = array[valid_size--];
        down_adjust(1, valid_size);
    }
    return;
}
void time_heap::tick()
{
    heap_timer *temp = array[1];
    time_t cur = time(NULL);
    printf("tick once, cur:%ld\n", cur);
    while (temp)
    {
        if (!temp)
            break;
        if (temp->expire > cur)
            break;
        temp->cb_func(temp->user);
        pop_timer();
        temp = array[1];
    }
    if (valid_size >= 1)
    {
        alarm(array[1]->expire - cur);
        printf("alarm:%ld\n", array[1]->expire - cur);
    }

    alarm(1);
}

void time_heap::up_adjust(int low, int high)
{
    int i = high, j = i / 2;
    while (j >= low)
    {
        if (array[j]->expire > array[i]->expire)
        {
            swap(array[j]->index, array[i]->index);
            swap(array[j], array[i]);
            i = j;
            j = i / 2;
        }
        else
        {
            break;
        }
    }
}
void time_heap::down_adjust(int low, int high)
{
    int i = low, j = i * 2;
    while (j <= high)
    {
        if ((j + 1) <= high && array[j]->expire > array[j + 1]->expire)
            j = j + 1;

        if (array[i]->expire > array[j]->expire)
        {
            swap(array[i]->index, array[i]->index);
            swap(array[i], array[j]);
            i = j;
            j = i * 2;
        }
        else
            break;
    }
}