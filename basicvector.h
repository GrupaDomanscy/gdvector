#ifndef BASICVECTOR_VECTOR_H_
#define BASICVECTOR_VECTOR_H_

#define BASICVECTOR_SUCCESS 0
#define BASICVECTOR_MEMORY_ERROR -1
#define BASICVECTOR_ITEM_NOT_FOUND -2

struct basicvector_s;

typedef bool (*basicvector_search_function)(void *user_data, void *item);

// Initialize basicvector
int basicvector_init(struct basicvector_s **vector);

int basicvector_push(struct basicvector_s *vector, void *item);

int basicvector_get(struct basicvector_s *vector, int index, void **result);

int basicvector_find_index(
    struct basicvector_s *vector, 
    int *result,
    basicvector_search_function search_function, 
    void *user_data
);

int basicvector_length(struct basicvector_s *vector);

int basicvector_set(struct basicvector_s *vector, int index, void *item);

int basicvector_remove(struct basicvector_s *vector, int index);

void basicvector_free(struct basicvector_s *vector);

#endif //BASICVECTOR_VECTOR_H_
