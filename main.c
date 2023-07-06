#include <criterion/criterion.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gdalloc.h>
#include "gdvector.h"

int dispatches = 0;

void remove_action(void *item) {
    dispatches++;
    free(item);
}

Test(gdvector_init, initializes_structure_successfully) {
    gdvector *vector = gdvector_init(NULL);
    cr_assert(vector->items == NULL, "vector->items should be null.");
    cr_assert(vector->remove_action != NULL, "vector->remove_action should not be NULL.");
    cr_assert(vector->allocated_items == 0, "vector->allocated_items should be equal to 0.");
}

Test(gdvector_init, points_to_user_defined_remove_action) {
    void *remove_action_pointer = &remove_action;

    gdvector *vector = gdvector_init(remove_action_pointer);
    cr_assert(vector->items == NULL, "vector->items should be null.");
    cr_assert(vector->remove_action == remove_action_pointer,
              "vector->remove_action should be equal to remove_action_pointer variable.");
    cr_assert(vector->allocated_items == 0, "vector->allocated_items should be equal to 0.");
}

Test(gdvector_push_back, adds_item_to_the_end_of_the_array_successfully) {
    gdvector *vector = gdvector_init(NULL);

    int **items = safe_malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++) items[i] = safe_malloc(sizeof(int));
    *(items[0]) = 72;
    *(items[1]) = 15;
    *(items[2]) = 24;

    gdvector_push_back(vector, items[0]);
    gdvector_push_back(vector, items[1]);
    gdvector_push_back(vector, items[2]);

    for (int i = 0; i < 3; i++) {
        cr_assert(vector->items[i] == items[i],
                  "Pointer inside structure should point to the same pointer in items array.");
        cr_assert(*((int *) (vector->items[i])) == *(items[i]),
                  "Value inside pointer structure should be the same as in pointer in items array.");
    }

    cr_assert(vector->allocated_items == 3, "Allocated items should be equal to 3.");
}

Test(gdvector_remove, removes_items_successfully) {
    srand(time(NULL));
    gdvector *vector = gdvector_init(NULL);

    int **items = safe_malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++) items[i] = safe_malloc(sizeof(int));
    *(items[0]) = 72;
    *(items[1]) = 15;
    *(items[2]) = 24;

    gdvector_push_back(vector, items[0]);
    gdvector_push_back(vector, items[1]);
    gdvector_push_back(vector, items[2]);

    int result = gdvector_remove(vector, 0);
    cr_assert(result == 0);

    cr_assert(vector->allocated_items == 2, "Allocated items should be equal to 2.");

    for (int i = 0; i < 2; i++) {
        cr_assert(vector->items[i] == items[i + 1],
                  "Pointer inside structure should point to the same pointer in items array.");
        cr_assert(*((int *) (vector->items[i])) == *(items[i + 1]),
                  "Value inside pointer structure should be the same as in pointer in items array.");
    }
}

Test(gdvector_remove, returns_minus_one_if_index_argument_is_invalid) {
    srand(time(NULL));
    gdvector *vector = gdvector_init(NULL);

    int **items = safe_malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++) items[i] = safe_malloc(sizeof(int));
    *(items[0]) = 72;
    *(items[1]) = 15;
    *(items[2]) = 24;

    gdvector_push_back(vector, items[0]);
    gdvector_push_back(vector, items[1]);
    gdvector_push_back(vector, items[2]);

    cr_assert(gdvector_remove(vector, 3) == -1, "gdvector_remove should return -1 on invalid index");
    cr_assert(gdvector_remove(vector, -1) == -1, "gdvector_remove should return -1 on invalid index (second try)");

    for (int i = 0; i < 3; i++) {
        cr_assert(vector->items[i] == items[i],
                  "Pointer inside structure should point to the same pointer in items array.");
        cr_assert(*((int *) (vector->items[i])) == *(items[i]),
                  "Value inside pointer structure should be the same as in pointer in items array.");
    }

    cr_assert(vector->allocated_items == 3, "allocated items should be equal to 3.");
}

Test(gdvector_remove, deallocates_internal_array_when_there_are_no_items_after_removal) {
    srand(time(NULL));
    gdvector *vector = gdvector_init(&remove_action);

    int *item = safe_malloc(sizeof(int));
    *item = 72;

    gdvector_push_back(vector, item);

    gdvector_remove(vector, 0);

    cr_assert(vector->allocated_items == 0, "Allocated items should equal to 0.");
    cr_assert(vector->items == NULL, "Internal items should be null.");
}

Test(gdvector_remove, dispatches_remove_action_succesfully) {
    srand(time(NULL));
    gdvector *vector = gdvector_init(&remove_action);

    int *item = safe_malloc(sizeof(int));
    *item = 72;

    gdvector_push_back(vector, item);

    int old_dispatches = dispatches;
    gdvector_remove(vector, 0);
    cr_assert(dispatches == old_dispatches + 1, "Dispatches should be incremented by one.");
}

Test(gdvector_set, sets_item_on_index_successfully) {
    gdvector *vector = gdvector_init(NULL);

    int **items = safe_malloc(sizeof(int *) * 3);
    for (int i = 0; i < 3; i++) items[i] = safe_malloc(sizeof(int));
    *(items[0]) = 72;
    *(items[1]) = 15;
    *(items[2]) = 24;

    gdvector_push_back(vector, items[0]);

    int *override1 = safe_malloc(sizeof(int));
    *override1 = 9999;

    int result = gdvector_set(vector, 0, override1);

    cr_assert(result == 0, "Value returned from function should equal to 0");
    cr_assert(vector->allocated_items == 1, "Allocated items should be equal to 1.");
    cr_assert(vector->items[0] == override1, "Pointer in structure should be the same as pointer named 'override1'.");
    cr_assert(*((int *) (vector->items[0])) == *override1,
              "Pointer value in structure should be the same as pointer value in pointer named 'override1'.");
}

Test(gdvector_set, dispatches_remove_action_succesfully) {
    srand(time(NULL));
    gdvector *vector = gdvector_init(&remove_action);

    int *item = safe_malloc(sizeof(int));
    *item = 72;

    gdvector_push_back(vector, item);

    int old_dispatches = dispatches;

    int *override = safe_malloc(sizeof(int));
    *override = 80;

    gdvector_set(vector, 0, override);
    cr_assert(old_dispatches + 1 == dispatches, "Dispatches should be incremented.");
}

Test(gdvector_free, deallocates_structure) {
    gdvector *vector = gdvector_init(&remove_action);

    int *item1 = safe_malloc(sizeof(int));
    int *item2 = safe_malloc(sizeof(int));
    *item1 = 72;
    *item2 = 76;

    gdvector_push_back(vector, item1);
    gdvector_push_back(vector, item2);

    int old_dispatches = dispatches;

    gdvector_free(vector);

    cr_assert(old_dispatches + 2 == dispatches, "Dispatches should be incremented by two.");
}

Test(gdvector_get, gets_item_successfully) {
    gdvector *vector = gdvector_init(NULL);

    int *item1 = safe_malloc(sizeof(int));
    int *item2 = safe_malloc(sizeof(int));
    *item1 = 72;
    *item2 = 76;

    gdvector_push_back(vector, item1);
    gdvector_push_back(vector, item2);

    int *ptr1 = gdvector_get(vector, 0);
    int *ptr2 = gdvector_get(vector, 1);

    cr_assert(*ptr1 == 72);
    cr_assert(*ptr2 == 76);
}

Test(gdvector_get, returns_null_if_index_is_invalid) {
    gdvector *vector = gdvector_init(NULL);

    int *item1 = safe_malloc(sizeof(int));
    int *item2 = safe_malloc(sizeof(int));
    *item1 = 72;
    *item2 = 76;

    gdvector_push_back(vector, item1);
    gdvector_push_back(vector, item2);

    int *ptr1 = gdvector_get(vector, -1);
    int *ptr2 = gdvector_get(vector, 3);

    cr_assert(ptr1 == NULL);
    cr_assert(ptr2 == NULL);
}

Test(gdvector_length, gets_length_successfully) {
    gdvector *vector = gdvector_init(NULL);

    int *item1 = safe_malloc(sizeof(int));
    int *item2 = safe_malloc(sizeof(int));
    *item1 = 72;
    *item2 = 76;

    cr_assert(gdvector_length(vector) == 0, "Vector length should be equal to 0.");
    gdvector_push_back(vector, item1);
    cr_assert(gdvector_length(vector) == 1, "Vector length should be equal to 1.");
    gdvector_push_back(vector, item2);
    cr_assert(gdvector_length(vector) == 2, "Vector length should be equal to 2.");
    gdvector_remove(vector, 0);
    cr_assert(gdvector_length(vector) == 1, "Vector length should be equal to 1.");
    gdvector_remove(vector, 0);
    cr_assert(gdvector_length(vector) == 0, "Vector length should be equal to 0.");
}