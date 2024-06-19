#include "lprofiler.h"
#include <stdio.h>
#include <assert.h>

#define MAX_INSTANCES (MAX_LPROFILER_INSTANCES)

lprofiler_get_ticks_t *fpGetTicks;
float TicksPerMs;
lprofiler_t Instances[MAX_INSTANCES];
int InstanceCount = 0;

void lprofiler_init(lprofiler_get_ticks_t *get_ticks, float ticks_per_ms)
{
    fpGetTicks = get_ticks;
    TicksPerMs = ticks_per_ms;
    lprofiler_stats_clear_all();
    InstanceCount = 0;
}

lprofiler_t* lprofiler_add(const char *label)
{
    lprofiler_t *result = NULL;

    if(InstanceCount < MAX_INSTANCES)
    {
        Instances[InstanceCount].label = label;
        result = &Instances[InstanceCount];
        InstanceCount++;
    }

    assert(result);
    return result;
}

void lprofiler_start(lprofiler_t *instance)
{
    if(fpGetTicks)
        instance->start_tick = fpGetTicks();
}

void lprofiler_end(lprofiler_t *instance)
{
    if((fpGetTicks) && (instance->start_tick > 0))
    {
        instance->count++;
        uint32_t tdiff = fpGetTicks()- instance->start_tick;
        instance->tot_ticks += tdiff;
        if(instance->max_ticks < tdiff)
            instance->max_ticks = tdiff;
    }
}

size_t lprofiler_stats_tostr(lprofiler_t *instance, char *str, size_t max_len)
{
    assert(instance);

    if(instance->count > 0)
    {
        uint64_t avg_ticks = instance->tot_ticks / instance->count;
        return snprintf(str, max_len, "%s: Avg %fms (%llu), Max %fms (%u)\n",
                        instance->label,
                        avg_ticks / TicksPerMs, avg_ticks,
                        (float)instance->max_ticks/TicksPerMs, instance->max_ticks);
    }
    return 0;
}

size_t lprofiler_stats_tostr_all(char *str, size_t max_len)
{
    size_t n = 0;
    for (int i = 0; i < InstanceCount; i++)
    {
        n += lprofiler_stats_tostr(&Instances[i], str, max_len - n);
        if (n >= max_len)
            break;
    }
    return n;
}

void lprofiler_stats_print(lprofiler_t *instance)
{
    assert(instance);

    if(instance->count > 0)
    {
        uint64_t avg_ticks = instance->tot_ticks / instance->count;
        printf("%s: Avg %fms (%llu), Max %fms (%u)\n",
                      instance->label,
                      avg_ticks / TicksPerMs, avg_ticks,
                      (float)instance->max_ticks/TicksPerMs, instance->max_ticks);
    }
}

void lprofiler_stats_print_all(void)
{
    for (int i = 0; i < InstanceCount; i++)
    {
        lprofiler_stats_print(&Instances[i]);
    }
}

void lprofiler_stats_clear(lprofiler_t *instance)
{
    assert(instance);

    /* Clear everything except the label */
    lprofiler_t tmp = { instance->label };
    *instance = tmp;
}

void lprofiler_stats_clear_all(void)
{
    for (int i = 0; i < InstanceCount; i++)
    {
        lprofiler_stats_clear(&Instances[i]);
    }
}

