#ifndef __LPROFILER_H
#define __LPROFILER_H

#include <stdint.h>
#include <stddef.h>

#define MAX_LPROFILER_INSTANCES (20)

typedef struct lprofiler_t
{
    const char *label;
    uint32_t start_tick;
    uint32_t max_ticks;
    uint64_t tot_ticks;
    uint32_t count;
} lprofiler_t;

typedef uint32_t lprofiler_get_ticks_t(void);

void lprofiler_init(lprofiler_get_ticks_t *get_ticks, float ticks_per_ms);
lprofiler_t* lprofiler_add(const char *label);
void lprofiler_start(lprofiler_t *instance);
void lprofiler_end(lprofiler_t *instance);
size_t lprofiler_stats_tostr(lprofiler_t *instance, char *str, size_t max_len);
size_t lprofiler_stats_tostr_all(char *str, size_t max_len);
void lprofiler_stats_print(lprofiler_t *instance);
void lprofiler_stats_print_all(void);
void lprofiler_stats_clear(lprofiler_t *instance);
void lprofiler_stats_clear_all(void);

#define LPROFILE(label, f) do { static lprofiler_t *p; if(!p) p = lprofiler_add(label); lprofiler_start(p); f; lprofiler_end(p); } while(0)

#endif /* __LPROFILER_H */
