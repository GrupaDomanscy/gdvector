# GDVECTOR

Pointer value only vector implementation used internally by Grupa Domańscy company.

## WARNING

This library uses [GDALLOC](https://github.com/GrupaDomanscy/gdalloc) under the hood. It is going to terminate your
program if `malloc`, `calloc` or `realloc` function returns NULL pointer (computer out of memory).

## Installation

You have to install dependencies first:

- [GDALLOC](https://github.com/GrupaDomanscy/gdalloc)
- [CRITERION](https://github.com/Snaipe/Criterion)

Then you run `make` and `sudo make install`

Installation process tested on Fedora Workstation 37 x86_64 KDE Plasma  
Kernel: 6.3.7-100.fc37.x86_64

## How it works?

1. Initialize vector, in the first argument provide a pointer to function that will deallocate the item that needs to be removed.
There is a default function that is the same as `remove_action` func in this example, so you can provide NULL too.
```c
#include <gdalloc.h>
#include <gdvector.h>

// default remove action (already defined in lib)
void remove_action(void *item) {
    free(item);
}

int main() {
    gdvector *vector = gdvector_init(NULL);
    // or
    gdvector *vector = gdvector_init(&remove_action);

    return 0;
}
```

2. Have fun!
```c
#include <gdalloc.h>
#include <gdvector.h>

int main() {
    gdvector *vector = gdvector_init(NULL);
    
    int *item1 = safe_malloc(sizeof(int));
    int *item2 = safe_malloc(sizeof(int));
    
    *item1 = 999;
    *item2 = 998;
    
    gdvector_push_back(vector, item1);
    gdvector_push_back(vector, item2);
    
    int *ptr1 = gdvector_get(vector, 0);
    int *ptr2 = gdvector_get(vector, 1);
    
    // ptr1 == item1
    // ptr2 == item2
    
    // remove item1  - - - - - array before removal: [item1, item2]
    gdvector_remove(vector, 0);
    // remove item2  - - - - - array before removal: [item2]
    gdvector_remove(vector, 0);
    
    gdvector_free(vector);
    
    return 0;
}
```
