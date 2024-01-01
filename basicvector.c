#include <stdio.h>
#include <stdlib.h>
#include "basicvector.h"

struct basicvector_s {
    struct basicvector_entry_s *starting_entry;
    int cached_length;
};

struct basicvector_entry_s {
    void *item;
    struct basicvector_entry_s *next_entry;
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
    if (vector == NULL) return BASICVECTOR_MEMORY_ERROR;

    struct basicvector_entry_s *entry = malloc(sizeof(struct basicvector_entry_s));

    if (entry == NULL) {
        return BASICVECTOR_MEMORY_ERROR;
    }

    vector->cached_length++;

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
    if (vector == NULL) return BASICVECTOR_MEMORY_ERROR;
    
    if (index > vector->cached_length - 1) {
        *result = NULL;
        return BASICVECTOR_ITEM_NOT_FOUND;
    }

    struct basicvector_entry_s *entry = vector->starting_entry;

    for (int i = 1; i <= index; i++) {
        entry = entry->next_entry;
    }

    *result = entry->item;
    return BASICVECTOR_SUCCESS;
}

int basicvector_find_index(
    struct basicvector_s *vector,
    int *result,
    bool (*search_function)(void *item, void *user_data),
    void *user_data
) {
    if (vector == NULL) return BASICVECTOR_MEMORY_ERROR;
    if (search_function == NULL || result == NULL) return BASICVECTOR_INVALID_ARGUMENT;

    if (vector->starting_entry == NULL) {
        return BASICVECTOR_ITEM_NOT_FOUND;
    }

    int i = 0;
    struct basicvector_entry_s *examined_entry = vector->starting_entry;

    while (examined_entry != NULL) {
        if (search_function(examined_entry->item, user_data)) {
            *result = i;
            return BASICVECTOR_SUCCESS;
        }

        examined_entry = examined_entry->next_entry;
        i++;
    }

    return BASICVECTOR_ITEM_NOT_FOUND;
}

int basicvector_find(
    struct basicvector_s *vector, 
    void **result,
    bool (*search_function)(void *item, void *user_data),
    void *user_data
) {
    if (vector == NULL) return BASICVECTOR_MEMORY_ERROR;
    if (result == NULL || search_function == NULL) return BASICVECTOR_INVALID_ARGUMENT;

    if (vector->starting_entry == NULL) {
        *result = NULL;
        return BASICVECTOR_ITEM_NOT_FOUND;
    }

    struct basicvector_entry_s *examined_entry = vector->starting_entry;

    while (examined_entry != NULL) {
        if (search_function(examined_entry->item, user_data)) {
            *result = examined_entry->item;
            return BASICVECTOR_SUCCESS;
        }

        examined_entry = examined_entry->next_entry;
    }

    *result = NULL;
    return BASICVECTOR_ITEM_NOT_FOUND;
}

int basicvector_length(struct basicvector_s *vector, int *result) {
    if (vector == NULL) {
        return BASICVECTOR_MEMORY_ERROR;
    }

    if (result == NULL) return BASICVECTOR_INVALID_ARGUMENT;

    *result = vector->cached_length;

    return BASICVECTOR_SUCCESS;
}

int basicvector_set(
    struct basicvector_s *vector, 
    int index,
    void *item,
    void (*deallocation_function)(void* item, void *user_data),
    void *user_data
) {
    if (vector == NULL) return BASICVECTOR_MEMORY_ERROR;

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

            vector->cached_length++;
        } else {
            if (deallocation_function != NULL) {
                deallocation_function(entry_to_affect->item, user_data);
            }
            entry_to_affect->item = item;
        }

        return BASICVECTOR_SUCCESS;
    }

    if (vector->starting_entry == NULL) {
        vector->starting_entry = basicvector_internal_new_entry(NULL);

        if (vector->starting_entry == NULL) {
            return BASICVECTOR_MEMORY_ERROR;
        }

        vector->cached_length++;

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

            vector->cached_length++;
        }

        examined_entry = examined_entry->next_entry;
    }

    if (examined_entry->item != NULL && deallocation_function != NULL) {
        deallocation_function(examined_entry->item, user_data);
    }

    examined_entry->item = item;

    return BASICVECTOR_SUCCESS;
}

int basicvector_remove(
    struct basicvector_s *vector, 
    int index, 
    void (*deallocation_function)(void* item, void *user_data),
    void *user_data
) {
    if (vector == NULL) return BASICVECTOR_MEMORY_ERROR;

    int length;
    
    int status = basicvector_length(vector, &length);
    if (status != BASICVECTOR_SUCCESS) {
        return status;
    }

    if (index < 0 || index + 1 > length) {
        return BASICVECTOR_INVALID_INDEX;
    }

    if (index == 0) {
        struct basicvector_entry_s *entry_to_affect = vector->starting_entry;

        if (entry_to_affect == NULL) {
            return BASICVECTOR_INVALID_INDEX;
        }

        if (deallocation_function != NULL) {
            deallocation_function(entry_to_affect->item, user_data);
        }

        vector->starting_entry = entry_to_affect->next_entry;

        free(entry_to_affect);

        vector->cached_length--;

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

    if (deallocation_function != NULL) {
        deallocation_function(entry_to_remove->item, user_data);
    }
    
    free(entry_to_remove);

    vector->cached_length--;

    return BASICVECTOR_SUCCESS;
}

int basicvector_free(struct basicvector_s *vector, void (*deallocation_function)(void *item, void *user_data), void *user_data) {
    if (vector == NULL) {
        return BASICVECTOR_MEMORY_ERROR;
    }

    struct basicvector_entry_s* entry = vector->starting_entry;

    while (entry != NULL) {
        struct basicvector_entry_s *next_entry = entry->next_entry;

        if (deallocation_function != NULL) {
            deallocation_function(entry->item, user_data);
        }

        free(entry);

        entry = next_entry;
    }

    free(vector);

    return BASICVECTOR_SUCCESS;
}
