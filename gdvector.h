#ifndef GDVECTOR_VECTOR_H_
#define GDVECTOR_VECTOR_H_

typedef struct {
    int allocated_items;
    void **items;

    void (*remove_action)(void *item);
} gdvector;

gdvector *gdvector_init(void (*remove_action)(void *item));

void gdvector_push_back(gdvector *vector, void *item);

void *gdvector_get(gdvector *vector, int index);

int gdvector_set(gdvector *vector, int index, void *item);

int gdvector_remove(gdvector *vector, int index);

void gdvector_free(gdvector *vector);

#endif //GDVECTOR_VECTOR_H_
