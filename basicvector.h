#ifndef BASICVECTOR_VECTOR_H_
#define BASICVECTOR_VECTOR_H_

struct basicvector_s;

typedef bool (*basicvector_search_function)(void *user_data, void *item);

// Initialize basicvector
int basicvector_init(struct basicvector_s **vector);

int basicvector_push(struct basicvector_s *vector, void *item);

void *basicvector_get(struct basicvector_s *vector, int index);

int basicvector_find_index(
    struct basicvector_s *vector, 
    void *searched_ptr, 
    basicvector_search_function search_function, 
    void *user_data
);

int basicvector_length(struct basicvector_s *vector);

int basicvector_set(struct basicvector_s *vector, int index, void *item);

int basicvector_remove(struct basicvector_s *vector, int index);

void basicvector_free(struct basicvector_s *vector);

#endif //BASICVECTOR_VECTOR_H_
