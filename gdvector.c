#include <stddef.h>
#include <string.h>
#include <gdalloc.h>
#include "gdvector.h"

void default_remove_action(void *item) {
    free(item);
}

gdvector *gdvector_init(void (*remove_action)(void *item)) {
    gdvector *vector = safe_malloc(sizeof(gdvector));
    vector->items = NULL;
    vector->allocated_items = 0;

    if (remove_action == NULL) {
        remove_action = default_remove_action;
    }

    vector->remove_action = remove_action;
    return vector;
}

void *gdvector_get(gdvector *vector, int index) {
    if (index < 0 || index > vector->allocated_items - 1) return NULL;

    return vector->items[index];
}

void gdvector_push_back(gdvector *vector, void *item) {
    if (vector->allocated_items == 0) {
        // If there is nothing allocated (no items in vector)
        vector->items = safe_malloc(8);
        vector->allocated_items = 1;
        vector->items[0] = item;
    } else {
        // If there is something allocated (items in vector)
        vector->allocated_items += 1;
        vector->items = safe_realloc(vector->items, vector->allocated_items * 8);
        vector->items[vector->allocated_items - 1] = item;
    }
}

int gdvector_set(gdvector *vector, int index, void *item) {
    if (index > vector->allocated_items - 1 || index <= -1) return -1;

    vector->remove_action(vector->items[index]);
    vector->items[index] = item;

    return 0;
}

int gdvector_remove(gdvector *vector, int index) {
    // if index is out of every possible values, return -1
    if (index < 0) return -1;
    if (index > vector->allocated_items - 1) return -1;

    if (vector->allocated_items == 1) {
        // If index is the last item and there is only one item in vector, deallocate last item,
        // internal array and set allocated items to 0.
        vector->allocated_items = 0;
        vector->remove_action(vector->items[0]);
        free(vector->items);
        vector->items = NULL;

        return 0;
    } else if (index == vector->allocated_items - 1) {
        // If index is the last item, but not only one in vector, deallocate lsat item and
        // reallocate internal array to smaller size
        vector->allocated_items -= 1;
        vector->remove_action(vector->items[0]);
        vector->items = safe_realloc(vector->items, vector->allocated_items * 8);

        return 0;
    } else {
        vector->remove_action(vector->items[index]);

        memmove(vector->items + index,
                vector->items + index + 1,
                (vector->allocated_items - index - 1) * 8);

        vector->allocated_items -= 1;

        vector->items = safe_realloc(vector->items, vector->allocated_items * 8);

        return 0;
    }
}

void gdvector_free(gdvector *vector) {
    for (int i = 0; i < vector->allocated_items; i++) {
        vector->remove_action(vector->items[i]);
    }

    free(vector->items);
    free(vector);
}