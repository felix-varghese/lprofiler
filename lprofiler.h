#ifndef __LPROFILER_H
#define __LPROFILER_H

#include <stdint.h>
#include <stddef.h>

// Maximum number of profiling instances supported
#define MAX_LPROFILER_INSTANCES (20)

// Profiler structure to hold profiling data
typedef struct lprofiler_t
{
    const char *label;        // Label for the profiling instance
    uint32_t start_tick;      // Start tick count
    uint32_t max_ticks;       // Maximum ticks recorded
    uint64_t tot_ticks;       // Total ticks accumulated
    uint32_t count;           // Number of times the instance has been profiled
} lprofiler_t;

// Function pointer type for getting the current tick count
typedef uint32_t lprofiler_get_ticks_t(void);

/**
 * @brief Initializes the profiler library.
 *
 * @param get_ticks Function pointer to get the current tick count.
 * @param ticks_per_ms Number of ticks per millisecond.
 */
void lprofiler_init(lprofiler_get_ticks_t *get_ticks, float ticks_per_ms);

/**
 * @brief Adds a new profiling instance.
 *
 * @param label The label for the new profiling instance.
 * @return lprofiler_t* Pointer to the new profiling instance.
 */
lprofiler_t* lprofiler_add(const char *label);

/**
 * @brief Starts profiling for the given instance.
 *
 * @param instance The profiling instance to start.
 */
void lprofiler_start(lprofiler_t *instance);

/**
 * @brief Ends profiling for the given instance and updates the profiling data.
 *
 * @param instance The profiling instance to end.
 */
void lprofiler_end(lprofiler_t *instance);

/**
 * @brief Converts the statistics of a profiling instance to a string.
 *
 * @param instance The profiling instance.
 * @param str The string buffer to store the result.
 * @param max_len The maximum length of the string buffer.
 * @return size_t The number of characters written to the string buffer.
 */
size_t lprofiler_stats_tostr(lprofiler_t *instance, char *str, size_t max_len);

/**
 * @brief Converts the statistics of all profiling instances to a string.
 *
 * @param str The string buffer to store the result.
 * @param max_len The maximum length of the string buffer.
 * @return size_t The number of characters written to the string buffer.
 */
size_t lprofiler_stats_tostr_all(char *str, size_t max_len);

/**
 * @brief Prints the statistics of a profiling instance to the standard output.
 *
 * @param instance The profiling instance to print.
 */
void lprofiler_stats_print(lprofiler_t *instance);

/**
 * @brief Prints the statistics of all profiling instances to the standard output.
 */
void lprofiler_stats_print_all(void);

/**
 * @brief Clears the statistics of a profiling instance.
 *
 * @param instance The profiling instance to clear.
 */
void lprofiler_stats_clear(lprofiler_t *instance);

/**
 * @brief Clears the statistics of all profiling instances.
 */
void lprofiler_stats_clear_all(void);

/**
 * @brief Macro to profile a block of code.
 *
 * @param label The label for the profiling instance.
 * @param f The block of code to be profiled.
 */
#define LPROFILE(label, f) do { static lprofiler_t *p; if(!p) p = lprofiler_add(label); lprofiler_start(p); f; lprofiler_end(p); } while(0)

#endif /* __LPROFILER_H */
