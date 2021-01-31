#include "timer_tw.h"

time_wheel::time_wheel() : cur_slot(0)
{
    for (int i = 0; i < N; i++)
        slots[i] = NULL;
}
time_wheel::~time_wheel()
{
    for (int i = 0; i < N; i++)
    {
        tw_timer *temp = NULL;
        while (slots[i])
        {
            temp = slots[i];
            slots[i] = temp->next;
            delete temp;
        }
    }
}

int time_wheel::get_tick_SI()
{
    return SI;
}

tw_timer *time_wheel::add_timer(int timeout)
{
    if (timeout < 0)
        return NULL;

    int tick = 0;
    if (timeout < 1)
        tick = 1;
    else
        tick = timeout / SI;

    int rotation = tick / N;
    int time_slot = (cur_slot + tick % N) % N;
    tw_timer *timer = new tw_timer(rotation, time_slot);

    if (!slots[time_slot])
    {
        printf("add timer, rotation is %d, It will be processed in time_slot %d, cur_slot is %d\n", rotation, time_slot + 1, cur_slot);
        slots[time_slot] = timer;
    }
    else
    {
        timer->next = slots[time_slot];
        slots[time_slot]->prev = timer;
        slots[time_slot] = timer;
    }
    return timer;
}
void time_wheel::del_timer(tw_timer *timer)
{
    if (!timer)
        return;

    int time_slot = timer->time_slot;
    if (timer == slots[time_slot])
        slots[time_slot] = timer->next;
    else
        timer->prev->next = timer->next;
    if (timer->next)
        timer->next->prev = timer->prev;

    delete timer;
}
void time_wheel::tick()
{
    tw_timer *temp = slots[cur_slot];
    printf("current slot %d is over\n", cur_slot);

    while (temp)
    {
        printf("tick the timer once, then ");
        if (temp->rotation > 0)
        {
            temp->rotation--;
            temp = temp->next;
        }
        else
        {
            temp->cb_func(temp->user_data);
            //删除方式1
            tw_timer *temp2 = temp->next;
            del_timer(temp);
            temp = temp2;

            //删除方式2
            // tw_timer *temp2 = temp->next;
            // if (temp == slots[cur_slot])
            //     slots[cur_slot] = temp->next;
            // else
            //     temp->prev->next = temp->next;

            // if (temp->next)
            //     temp->next->prev = temp->prev;
            // delete temp;
            // temp = temp2;
        }
    }
    cur_slot = (cur_slot + 1) % N;
    alarm(SI);
}