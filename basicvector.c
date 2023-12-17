#include <stdlib.h>
struct basicvector_s {
    void *starting_item;
    int cached_length;
};

int basicvector_init(struct basicvector_s **vector) {
    struct basicvector_s *new_vector = malloc(sizeof(struct basicvector_s));

    if (new_vector == NULL) {
        return -1;
    }

    new_vector->cached_length = 0;
    new_vector->starting_item = NULL;

    *vector = new_vector;

    return 0;
}

