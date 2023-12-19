#include <stdio.h>
#include "basicvector.h"

char *get_status_from_int(int status) {
    switch (status) {
        case 0:
            return "BASICVECTOR_SUCCESS";
        case -1:
            return "BASICVECTOR_MEMORY_ERROR";
        case -2:
            return "BASICVECTOR_ITEM_NOT_FOUND";
        case -3:
            return "BASICVECTOR_INVALID_INDEX";
        default:
            return "Unknown status";
    }
}

void expect_status_success(int status) {
    if (status != BASICVECTOR_SUCCESS) {
        printf(
            "[FAIL] Expected status to be BASICVECTOR_SUCCESS, received %s\n", 
            get_status_from_int(status)
        );
    } else {
        printf(
            "[PASS] Expected status to be BASICVECTOR_SUCCESS, received %s\n", 
            get_status_from_int(status)
        );
    }
}

void expect_item_to_be(struct basicvector_s *vector, int index, int *expected_value, char *identifier_name) {
    int *received;

    expect_status_success(basicvector_get(vector, index, (void **) &received));

    if (received != expected_value) {
        printf("[FAIL] Expected %s to be %p, received %p\n", identifier_name, expected_value, received);
    } else {
        printf("[PASS] Expected %s to be %p, received %p\n", identifier_name, expected_value, received);
    }
}

void deallocation_function(void *item) {
    // ...
}

int main() {
    struct basicvector_s *vector;
    int length;
    int status;

    int item1 = 56;
    int item2 = 57;
    int item3 = 58;

    int *item1expected = &item1;
    int *item2expected = &item2;
    int *item3expected = &item3;

    int *item1received;
    int *item2received;
    int *item3received;

    expect_status_success(basicvector_init(&vector));
    length = basicvector_length(vector);

    if (length != 0) {
        printf("[FAIL] Expected length to be 0, received: %d\n", length);
    } else {
        printf("[PASS] Expected length to be 0, received %d\n", length);
    }

    expect_status_success(basicvector_push(vector, item1expected));
    expect_status_success(basicvector_push(vector, item2expected));
    expect_status_success(basicvector_push(vector, item3expected));

    length = basicvector_length(vector);

    if (length != 3) {
        printf("[FAIL] Expected length to be 3, received %d\n", length);
    } else {
        printf("[PASS] Expected length to be 3, received %d\n", length);
    }

    expect_item_to_be(vector, 0, item1expected, "item1expected");
    expect_item_to_be(vector, 1, item2expected, "item2expected");
    expect_item_to_be(vector, 2, item3expected, "item3expected");

    basicvector_free(vector, deallocation_function);

    return 0;
}
