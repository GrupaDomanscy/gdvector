#ifndef GDVECTOR_VECTOR_H_
#define GDVECTOR_VECTOR_H_

typedef int (*search_function)(void *ptr, void *searched_ptr);

typedef struct {
    int allocated_items;
    void **items;

    void (*remove_action)(void *item);
} gdvector;

gdvector *gdvector_init(void (*remove_action)(void *item));

void gdvector_push_back(gdvector *vector, void *item);

void *gdvector_get(gdvector *vector, int index);

int gdvector_find_index(gdvector *vector, void *searched_ptr, search_function search_function);

int gdvector_length(gdvector *vector);

int gdvector_set(gdvector *vector, int index, void *item);

int gdvector_remove(gdvector *vector, int index);

void gdvector_free(gdvector *vector);

#endif //GDVECTOR_VECTOR_H_
