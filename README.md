# c-utils

Minimal C utilities mostly developed for myself, but maybe they'll be useful to someone else too.

## Features

- Header-only dynamic array (`cu_array.h`)
- Simple unit testing framework (`cu_test.h`)
- Only depends on libc, C99
    - You can remove libc dependency entirely by providing your own `memcpy`, `malloc`, etc.

## Getting Started

```c
#define CU_ARRAY_IMPL
#include "cu_array.h"

...

cu_array_t arr = {0};
cu_array_init(&arr, sizeof(int));
int x = 42;
cu_array_append(&arr, &x);
cu_array_deinit(&arr);
```

## Tests

Unit tests double as usage examples. To build and run them:

```
make
./build/test_cu_array
```

## Config

Check the top of header files for a list of #defines to customize the behavior.

## Disclaimer

There are still some bugs, but with the default configs the tests pass.
The tests don't cover everything, just the basic usage.
Also I tried using other `CU_UNIT` definitions, but they don't really work. 
Probably the pointers to the data should be cast down always to `unsigned char`.