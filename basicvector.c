#include <stdlib.h>
#include "basicvector.h"

struct basicvector_s {
    void *starting_entry;
    int cached_length;
};

struct basicvector_entry_s {
    void *item;
    void *next_entry;
};

struct basicvector_entry_s* basicvector_internal_new_entry(void *item) {
    struct basicvector_entry_s* new_entry = malloc(sizeof(struct basicvector_entry_s));

    if (new_entry == NULL) {
        return NULL;
    }

    new_entry->item = item;
    new_entry->next_entry = NULL;

    return new_entry; 
}

int basicvector_init(struct basicvector_s **vector) {
    struct basicvector_s *new_vector = malloc(sizeof(struct basicvector_s));

    if (new_vector == NULL) {
        return BASICVECTOR_MEMORY_ERROR;
    }

    new_vector->cached_length = 0;
    new_vector->starting_entry = NULL;

    *vector = new_vector;

    return BASICVECTOR_SUCCESS;
}

struct basicvector_entry_s *basicvector_internal_find_last_item(struct basicvector_s* vector) {
    if (vector->starting_entry == NULL) {
        return NULL;
    }

    struct basicvector_entry_s *examined_entry = vector->starting_entry;

    while (1) {
        if (examined_entry->next_entry == NULL) {
            return examined_entry;
        }

        examined_entry = examined_entry->next_entry;
    }

    return examined_entry;
}

int basicvector_push(struct basicvector_s *vector, void *item) {
    struct basicvector_entry_s *entry = malloc(sizeof(struct basicvector_entry_s));

    if (entry == NULL) {
        return BASICVECTOR_MEMORY_ERROR;
    }

    vector->cached_length += 1;

    entry->item = item;
    entry->next_entry = NULL;

    struct basicvector_entry_s *last_entry = basicvector_internal_find_last_item(vector);

    if (last_entry == NULL) {
        vector->starting_entry = entry;
    } else {
        last_entry->next_entry = entry;
    }

    return BASICVECTOR_SUCCESS;
}

int basicvector_get(struct basicvector_s *vector, int index, void **result) {
    if (index > vector->cached_length - 1) {
        *result = NULL;
        return BASICVECTOR_ITEM_NOT_FOUND;
    }

    struct basicvector_entry_s *entry = vector->starting_entry;

    for (int i = 1; i < index; i++) {
        entry = entry->next_entry;
    }

    *result = entry;
    return BASICVECTOR_SUCCESS;
}

int basicvector_find_index(
    struct basicvector_s *vector, 
    int *result,
    basicvector_search_function search_function,
    void *user_data
) {
    if (vector->starting_entry == NULL) {
        *result = BASICVECTOR_ITEM_NOT_FOUND;
        return BASICVECTOR_ITEM_NOT_FOUND;;
    }

    struct basicvector_entry_s *examined_entry = vector->starting_entry;

    for (int i = 0;;i++) {
        if (search_function(user_data, examined_entry->item)) {
            *result = BASICVECTOR_SUCCESS;
            return BASICVECTOR_SUCCESS;
        }

        examined_entry = examined_entry->next_entry;
    }

    *result = BASICVECTOR_ITEM_NOT_FOUND;
    return BASICVECTOR_ITEM_NOT_FOUND;
}

int basicvector_length(struct basicvector_s *vector) {
    return vector->cached_length;
}

int basicvector_set(
    struct basicvector_s *vector, 
    int index,
    void *item,
    void (*deallocation_function)(void* item)
) {
    if (index < 0) {
        return BASICVECTOR_INVALID_INDEX;
    }

    if (index == 0) {
        struct basicvector_entry_s *entry_to_affect = vector->starting_entry;

        if (entry_to_affect == NULL) {
            vector->starting_entry = basicvector_internal_new_entry(item);

            if (vector->starting_entry == NULL) {
                return BASICVECTOR_MEMORY_ERROR;
            }
        } else {
            deallocation_function(entry_to_affect->item);
            entry_to_affect->item = item;
        }

        return BASICVECTOR_SUCCESS;
    }

    struct basicvector_entry_s *examined_entry = vector->starting_entry;

    int i = 0;

    while (i != index) {
        i++;

        if (examined_entry->next_entry == NULL) {
            examined_entry->next_entry = basicvector_internal_new_entry(NULL);

            if (examined_entry->next_entry == NULL) {
                return BASICVECTOR_MEMORY_ERROR;
            }
        }

        examined_entry = examined_entry->next_entry;
    }

    if (examined_entry->item != NULL) {
        deallocation_function(examined_entry->item);
    }

    examined_entry->item = item;

    return BASICVECTOR_SUCCESS;
}

int basicvector_remove(
    struct basicvector_s *vector, 
    int index, 
    void (*deallocation_function)(void* item)
) {
    if (index < 0 || index + 1 > basicvector_length(vector)) {
        return BASICVECTOR_INVALID_INDEX;
    }

    if (index == 0) {
        struct basicvector_entry_s *entry_to_affect = vector->starting_entry;

        if (entry_to_affect == NULL) {
            return BASICVECTOR_INVALID_INDEX;
        }

        deallocation_function(entry_to_affect->item);

        vector->starting_entry = entry_to_affect->next_entry;

        free(entry_to_affect);

        return BASICVECTOR_SUCCESS;
    }

    struct basicvector_entry_s *entry_before = vector->starting_entry;

    int i = 0;

    while (i < index - 1) {
        i++;

        if (entry_before->next_entry == NULL) {
            return BASICVECTOR_INVALID_INDEX;
        }

        entry_before = entry_before->next_entry;
    }

    struct basicvector_entry_s *entry_to_remove = entry_before->next_entry;

    if (entry_to_remove == NULL) {
        return BASICVECTOR_INVALID_INDEX;
    }

    struct basicvector_entry_s *entry_after = entry_to_remove->next_entry;

    entry_before->next_entry = entry_after;

    deallocation_function(entry_to_remove->item);
    
    free(entry_to_remove);

    return BASICVECTOR_SUCCESS;
}