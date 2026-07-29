# darray

A lightweight, header-only dynamic array utility written in C.

## Features

- **Header-Only**: Easy to integrate, only requires including a single header file.

---

## Integration

To use darray, copy `mira/darray.h` to your project directory.

In **exactly one** source (`.c`) file, define `MIRA_DARRAY_IMPL` before including `darray.h` to instantiate the implementation:

```c
#define MIRA_DARRAY_IMPL
#include "mira/darray.h"
```

In other source files, simply include the header:

```c
#include "mira/darray.h"
```

---
## API Reference


---

## Usage Example

```c
#define MIRA_DARRAY_IMPL
#include "mira/darray.h"

int main(void) {
    

    return 0;
}
```
