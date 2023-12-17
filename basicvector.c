#include <stdlib.h>
#include <strings.h>

struct basicvector_s {
    void *starting_entry;
    int cached_length;
};

struct basicvector_entry_s {
    void *item;
    void *next_entry;
};

int basicvector_init(struct basicvector_s **vector) {
    struct basicvector_s *new_vector = malloc(sizeof(struct basicvector_s));

    if (new_vector == NULL) {
        return -1;
    }

    new_vector->cached_length = 0;
    new_vector->starting_entry = NULL;

    *vector = new_vector;

    return 0;
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
        return -1;
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

    return 0;
}

int basicvector_get(struct basicvector_s *vector, int index, void **result) {
    if (index > vector->cached_length - 1) {
        *result = NULL;
        return -1;
    }

    struct basicvector_entry_s *entry = vector->starting_entry;

    for (int i = 1; i < index; i++) {
        entry = entry->next_entry;
    }

    *result = entry;
    return 0;
}

int basicvector_find_index(
    struct basicvector_s *vector, 
    int *result,
    basicvector_search_function search_function, 
    void *user_data
) {
    if (vector->starting_entry == NULL) {
        *result = -1;
        return -1;
    }

    struct basicvector_entry_s *examined_entry = vector->starting_entry;

    for (int i = 0;;i++) {
        if (search_function(user_data, examined_entry->item)) {
            *result = i;
            return 0;
        }

        examined_entry = examined_entry->next_entry;
    }

    *result = -1;
    return -1;
}
