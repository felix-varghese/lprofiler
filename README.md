
# lprofiler

## Overview

`lprofiler` is a lightweight profiling library for C that allows you to measure and analyze the performance of different code blocks by tracking their execution times. It helps identify performance bottlenecks and optimize code efficiency.

## Features

- Easy-to-use functions for quickly profiling C code
- Measures average and maximum execution times
- Macro for convenient in-place profiling
- Easy to setup and use, no non-standard dependencies
- Easy to extend with your own functionality

## Installation

Simply include `lprofiler.h` and `lprofiler.c` in your project. No external dependencies are required.

## Usage

### Initialization

Initialize the profiler with a function to get the current tick count and the number of ticks per millisecond:

```c
#include "lprofiler.h"

// Define a function to get the current tick count
uint32_t get_ticks() {
    // Implement your tick function here
}

int main() {
    lprofiler_init(get_ticks, 1000.0f); // Assuming 1000 ticks per millisecond
    // Your code here
}
```

### Profiling a Code Block

Use the `LPROFILE` macro to profile a block of code without modifying the original function:

```c
void example_function(int arg) {
    // Your code here
}

int main() {
    lprofiler_init(get_ticks, 1000.0f);
    
    int ret;
    LPROFILE("example_function", ret = example_function(42));
    
    lprofiler_stats_print_all();
    return 0;
}
```

### Advanced Usage

For more advanced use cases, you can manually add, start, and end profiling:

```c
int main() {
    lprofiler_init(get_ticks, 1000.0f);
    
    lprofiler_t* lp = lprofiler_add("some code");
    
    lprofiler_start(lp);
    // some code to be profiled
    lprofiler_end(lp);
    
    lprofiler_stats_print(lp);
    return 0;
}
```

### Printing Statistics

Print the statistics of a specific instance or all instances:

```c
lprofiler_stats_print(profiler_instance);
lprofiler_stats_print_all();
```

### Clearing Statistics

Clear the statistics of a specific instance or all instances:

```c
lprofiler_stats_clear(profiler_instance);
lprofiler_stats_clear_all();
```

### Retrieving Statistics as Strings

Convert the statistics of a specific instance or all instances to a string:

```c
char buffer[256];
lprofiler_stats_tostr(profiler_instance, buffer, sizeof(buffer));
lprofiler_stats_tostr_all(buffer, sizeof(buffer));
```

## Example

The following example should work as-is when compiled in a linux/gcc environment.

Compile with:
```
gcc -o test test.c lprofiler.c
```

Code:
```c
#include "lprofiler.h"
#include <stdio.h>
#include <time.h>

// Function to get the current tick count
uint32_t get_ticks() {
    return (uint32_t)clock();
}

// Function to be profiled
int test_function(int arg) {
    int t = clock();
    while (clock() - t < (CLOCKS_PER_SEC*arg/1000));
    return arg;
}

int main() {
    // Initialize the profiler
    lprofiler_init(get_ticks, CLOCKS_PER_SEC/1000);

    // Profile the function
    int ret;

    for(int i = 0; i < 200; i++) {
        LPROFILE("test_function_19ms", ret = test_function(19));
        LPROFILE("test_function_30ms", ret = test_function(30));
        LPROFILE("test_function_1ms", ret = test_function(1));
    }

    // Print all profiling statistics
    lprofiler_stats_print_all();
    return 0;
}
```

Output:
```
test_function_19ms: Avg 19.000000ms (19000), Max 19.009001ms (19009)
test_function_30ms: Avg 30.000000ms (30000), Max 30.080000ms (30080)
test_function_1ms: Avg 1.000000ms (1000), Max 1.004000ms (1004)
```

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

`lprofiler` provides a straightforward and quick way to measure and analyze the performance of your C code, without needing a lot of overhead or setup time.
