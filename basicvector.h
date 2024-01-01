#ifndef BASICVECTOR_VECTOR_H_
#define BASICVECTOR_VECTOR_H_

#define BASICVECTOR_SUCCESS 0
#define BASICVECTOR_MEMORY_ERROR -1
#define BASICVECTOR_ITEM_NOT_FOUND -2
#define BASICVECTOR_INVALID_INDEX -3
#define BASICVECTOR_INVALID_ARGUMENT -4

#include <stdbool.h>

struct basicvector_s;

/*
 * Initialize vector structure
 *
 * Params:
 *  vector  - Pointer to pointer to vector structure
 *
 * Returns:
 *  BASICVECTOR_MEMORY_ERROR    - Returned if error occured while trying to allocate memory dynamically (if malloc failed)
 *  BASICVECTOR_SUCCESS         - If everything went ok
 */
int basicvector_init(struct basicvector_s **vector);

/*
 * Push item to vector structure
 *
 * Params:
 *  vector  - Pointer to vector structure
 *  item    - Item to push
 *
 * Returns:
 *  BASICVECTOR_MEMORY_ERROR    - returned if error occured while tried to allocate memory dynamically (if malloc failed)
 *  BASICVECTOR_SUCCESS         - returned if everything went ok
 */
int basicvector_push(struct basicvector_s *vector, void *item);

/*
 * Get item from basicvector structure
 *
 * Params:
 *  vector  - Pointer to vector structure
 *  index   - Index of item
 *  result  - Pointer to variable that will receive the item
 *
 * Returns:
 *  BASICVECTOR_MEMORY_ERROR    - returned if vector is NULL
 *  BASICVECTOR_ITEM_NOT_FOUND  - returned if item with given index does not exist
 *  BASICVECTOR_SUCCESS         - returned if everything went ok and value has been set to pointer under result argument
 */
int basicvector_get(struct basicvector_s *vector, int index, void **result);

/*
 * Find index of item in vector structure
 *
 * Params:
 *  vector          - Pointer to vector structure
 *  result          - Pointer to int variable, if everything will go ok, this variable will be equal to index of wanted item inside the vector
 *  search_function - search function callback. Every item will go through this function and if function returns true, this item's index will be assigned to result ptr (see result argument)
 *  user_data       - pointer to context data that will be passed to search_function, you can then use it to determine if item is correct to context data
 *
 * Returns:
 *  BASICVECTOR_MEMORY_ERROR        - returned if passed vector is NULL
 *  BASICVECTOR_INVALID_ARGUMENT    - returned if passed search_function or result is NULL
 *  BASICVECTOR_ITEM_NOT_FOUND      - returned if search function returned false on every item or if vector is empty
 *  BASICVECTOR_SUCCESS             - returned if everything went ok
 */
int basicvector_find_index(
    struct basicvector_s *vector,
    int *result,
    bool (*search_function)(void *item, void *user_data), 
    void *user_data
);

int basicvector_find(
    struct basicvector_s *vector, 
    void **result,
    bool (*search_function)(void *item, void *user_data), 
    void *user_data
);

int basicvector_length(struct basicvector_s *vector, int *result);

int basicvector_set(
    struct basicvector_s *vector, 
    int index,
    void *item,
    void (*deallocation_function)(void *item, void *user_data),
    void *user_data
);

int basicvector_remove(
    struct basicvector_s *vector, 
    int index, 
    void (*deallocation_function)(void* item, void *user_data),
    void *user_data
);

int basicvector_free(struct basicvector_s *vector, void (*deallocation_function)(void *item, void *user_data), void *user_data);

#endif //BASICVECTOR_VECTOR_H_
