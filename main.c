#include <stdio.h>
#include <stdlib.h>
#include "basicvector.h"

void assert(bool result, char *message) {
    if (!result) {
        printf("[FAIL] %s\n", message);
        exit(EXIT_FAILURE);
    }
}

void pass(char *message) {
    printf("[PASS] %s\n", message);
}

void expect_status_success(int status) {
    char *returned_status;

    switch (status) {
        case 0:
            returned_status = "BASICVECTOR_SUCCESS";
        case -1:
            returned_status = "BASICVECTOR_MEMORY_ERROR";
        case -2:
            returned_status = "BASICVECTOR_ITEM_NOT_FOUND";
        case -3:
            returned_status = "BASICVECTOR_INVALID_INDEX";
        default:
            returned_status = "Unknown status";
    }

    char *error_message = malloc(256);

    sprintf(error_message, "Expected status to be BASICVECTOR_SUCCESS, received %s", returned_status);

    assert(status == BASICVECTOR_SUCCESS, error_message);
}

void expect_item_to_be(struct basicvector_s *vector, int index, int *expected_value) {
    int *received;

    expect_status_success(basicvector_get(vector, index, (void **) &received));

    char *error_message = malloc(256);
    sprintf(error_message, "Expected %p to be equal to %p", received, expected_value);
    assert(received == expected_value, error_message);
}

void deallocation_function(void *item) {
    // ...
}

void test_if_basicvector_init_returns_valid_struct_pointer() {
    struct basicvector_s *vector;

    expect_status_success(basicvector_init(&vector));

    assert(vector != NULL, "Vector should not be null.");

    basicvector_free(vector, NULL);

    pass("basicvector_init returns valid struct pointer");
}

void test_if_basicvector_length_returns_valid_length() {
    struct basicvector_s *vector;

    expect_status_success(basicvector_init(&vector));

    assert(0 == basicvector_length(vector), "Expected length to be 0.");

    expect_status_success(basicvector_push(vector, NULL));
    expect_status_success(basicvector_push(vector, NULL));

    assert(2 == basicvector_length(vector), "Expected length to be 2.");
    
    basicvector_free(vector, deallocation_function);

    pass("basicvector_length returns valid length");
}

void test_if_basicvector_push_pushes_item_at_the_end_of_the_vector() {
    struct basicvector_s *vector;

    int item1 = 56;
    int item2 = 57;

    int *item1expected = &item1;
    int *item2expected = &item2;

    expect_status_success(basicvector_init(&vector));

    assert(0 == basicvector_length(vector), "Expected length to be 0.");

    expect_status_success(basicvector_push(vector, item1expected));
    expect_status_success(basicvector_push(vector, item2expected));
    expect_status_success(basicvector_push(vector, item1expected));

    assert(3 == basicvector_length(vector), "Expected length to be 3.");

    expect_item_to_be(vector, 0, item1expected);
    expect_item_to_be(vector, 1, item2expected);
    expect_item_to_be(vector, 2, item1expected);
    
    basicvector_free(vector, deallocation_function);

    pass("basicvector_push pushes item at the end of the vector");
}

void test_if_basicvector_set_sets_item_in_the_middle_of_the_vector() {
    struct basicvector_s *vector;

    int items[4] = { 56, 57, 58, 59 };

    int* item_references[4] = { &items[0], &items[1], &items[2], &items[3] };

    expect_status_success(basicvector_init(&vector));

    for (int i = 0; i < 3; i++) {
        expect_status_success(basicvector_push(vector, item_references[i]));
    }

    expect_status_success(basicvector_set(vector, 1, item_references[3], deallocation_function));

    expect_item_to_be(vector, 0, item_references[0]);
    expect_item_to_be(vector, 1, item_references[3]);
    expect_item_to_be(vector, 2, item_references[2]);

    basicvector_free(vector, deallocation_function);

    pass("basicvector_set sets item in the middle of the vector");
}

void test_if_basicvector_set_fills_non_existent_items_with_null_items() {
    struct basicvector_s *vector;

    int items[4] = { 56, 57, 58, 59 };

    int* item_references[4] = {
        &items[0],
        &items[1],
        &items[2],
        &items[3]
    };

    expect_status_success(basicvector_init(&vector));

    expect_status_success(basicvector_set(vector, 5, item_references[3], deallocation_function));

    expect_item_to_be(vector, 0, NULL);
    expect_item_to_be(vector, 1, NULL);
    expect_item_to_be(vector, 2, NULL);
    expect_item_to_be(vector, 3, NULL);
    expect_item_to_be(vector, 4, NULL);
    expect_item_to_be(vector, 5, item_references[3]);

    assert(basicvector_length(vector) == 6, "Length should be 6");

    basicvector_free(vector, deallocation_function);

    pass("basicvector_set fills non existent items with null items");
}

void test_if_basicvector_set_sets_first_item_when_no_items_are_inside_the_vector() {
    struct basicvector_s *vector;

    basicvector_init(&vector);

    assert(basicvector_length(vector) == 0, "Length should be 0");

    int item = 99;
    int *item_reference = &item;

    expect_status_success(basicvector_set(vector, 0, item_reference, NULL));

    expect_item_to_be(vector, 0, item_reference);

    assert(basicvector_length(vector) == 1, "Length should be 1");

    basicvector_free(vector, deallocation_function);

    pass("basicvector_set sets first item when no items are inside the vector");
}

void test_if_basicvector_set_sets_first_item_when_there_are_more_than_one_inside_the_vector() {
    struct basicvector_s *vector;

    basicvector_init(&vector);

    int item = 99;
    int *item_reference = &item;
    int *second_item_reference = &item;

    expect_status_success(basicvector_push(vector, second_item_reference));
    expect_status_success(basicvector_push(vector, second_item_reference));

    assert(basicvector_length(vector) == 2, "Length should be 2");

    expect_status_success(basicvector_set(vector, 0, item_reference, deallocation_function));

    expect_item_to_be(vector, 0, item_reference);
    expect_item_to_be(vector, 1, second_item_reference);

    assert(basicvector_length(vector) == 2, "Length should be 2");

    basicvector_free(vector, deallocation_function);

    pass("basicvector_set sets first item when there are more than one inside the vector");
}

int main() {
    test_if_basicvector_init_returns_valid_struct_pointer();
    test_if_basicvector_length_returns_valid_length();
    test_if_basicvector_push_pushes_item_at_the_end_of_the_vector();
    test_if_basicvector_set_sets_item_in_the_middle_of_the_vector();
    test_if_basicvector_set_fills_non_existent_items_with_null_items();
    test_if_basicvector_set_sets_first_item_when_no_items_are_inside_the_vector();
    test_if_basicvector_set_sets_first_item_when_there_are_more_than_one_inside_the_vector();
    
    pass("All passed");

    return 0;
}