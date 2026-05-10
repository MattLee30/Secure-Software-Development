#define _POSIX_C_SOURCE 200809L

#include "stack.h"
#include <stdio.h>
#include <string.h>

static int tests_run = 0;
static int tests_passed = 0;

static void check(int condition, const char *description) {
    tests_run++;
    if (condition) {
        tests_passed++;
        printf("PASS: %s\n", description);
    } else {
        printf("FAIL: %s\n", description);
    }
}

int main(void) {
    Stack *s = NULL;
    StackResponse r;

    r = stack_create(NULL);
    check(r.status == STACK_ERR_NULL_STACK, "stack_create(NULL) returns STACK_ERR_NULL_STACK");

    r = stack_create(&s);
    check(r.status == STACK_OK, "stack_create succeeds");
    check(s != NULL, "stack_create sets output pointer");

    r = stack_is_empty(s);
    check(r.status == STACK_OK && r.value == NULL, "new stack is empty");

    r = stack_size(s);
    check(r.status == STACK_OK && STACK_SIZE_VAL(r) == 0, "new stack has size 0");

    r = stack_pop(s);
    check(r.status == STACK_ERR_EMPTY, "pop on empty stack returns STACK_ERR_EMPTY");

    r = stack_peek(s);
    check(r.status == STACK_ERR_EMPTY, "peek on empty stack returns STACK_ERR_EMPTY");

    r = stack_push(s, NULL);
    check(r.status == STACK_ERR_NULL_STRING, "push NULL string returns STACK_ERR_NULL_STRING");

    r = stack_push(s, "hello");
    check(r.status == STACK_OK, "push 'hello' succeeds");

    r = stack_size(s);
    check(r.status == STACK_OK && STACK_SIZE_VAL(r) == 1, "size is 1 after push");

    r = stack_is_empty(s);
    check(r.status == STACK_OK && r.value != NULL, "stack is not empty after push");

    r = stack_peek(s);
    check(r.status == STACK_OK && strcmp(r.value, "hello") == 0, "peek returns 'hello'");
    stack_response_free_value(&r);

    r = stack_push(s, "world");
    check(r.status == STACK_OK, "push 'world' succeeds");

    r = stack_peek(s);
    check(r.status == STACK_OK && strcmp(r.value, "world") == 0, "peek returns top item 'world'");
    stack_response_free_value(&r);

    r = stack_size(s);
    check(r.status == STACK_OK && STACK_SIZE_VAL(r) == 2, "size is 2 after two pushes");

    r = stack_pop(s);
    check(r.status == STACK_OK && strcmp(r.value, "world") == 0, "pop returns 'world'");
    stack_response_free_value(&r);

    r = stack_pop(s);
    check(r.status == STACK_OK && strcmp(r.value, "hello") == 0, "pop returns 'hello' (LIFO order)");
    stack_response_free_value(&r);

    r = stack_is_empty(s);
    check(r.status == STACK_OK && r.value == NULL, "stack is empty after popping all");

    r = stack_push(NULL, "x");
    check(r.status == STACK_ERR_NULL_STACK, "push on NULL stack returns STACK_ERR_NULL_STACK");

    r = stack_pop(NULL);
    check(r.status == STACK_ERR_NULL_STACK, "pop on NULL stack returns STACK_ERR_NULL_STACK");

    r = stack_peek(NULL);
    check(r.status == STACK_ERR_NULL_STACK, "peek on NULL stack returns STACK_ERR_NULL_STACK");

    r = stack_size(NULL);
    check(r.status == STACK_ERR_NULL_STACK, "size on NULL stack returns STACK_ERR_NULL_STACK");

    r = stack_is_empty(NULL);
    check(r.status == STACK_ERR_NULL_STACK, "is_empty on NULL stack returns STACK_ERR_NULL_STACK");

    for (int i = 0; i < 20; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "item%d", i);
        r = stack_push(s, buf);
        check(r.status == STACK_OK, "push during capacity expansion succeeds");
    }

    r = stack_size(s);
    check(r.status == STACK_OK && STACK_SIZE_VAL(r) == 20, "size is 20 after 20 pushes");

    r = stack_pop(s);
    check(r.status == STACK_OK && strcmp(r.value, "item19") == 0, "pop returns last pushed item (LIFO)");
    stack_response_free_value(&r);

    check(strcmp(stack_status_string(STACK_OK), "STACK_OK") == 0, "status string STACK_OK");
    check(strcmp(stack_status_string(STACK_ERR_NULL_STACK), "STACK_ERR_NULL_STACK") == 0, "status string STACK_ERR_NULL_STACK");
    check(strcmp(stack_status_string(STACK_ERR_NULL_STRING), "STACK_ERR_NULL_STRING") == 0, "status string STACK_ERR_NULL_STRING");
    check(strcmp(stack_status_string(STACK_ERR_EMPTY), "STACK_ERR_EMPTY") == 0, "status string STACK_ERR_EMPTY");
    check(strcmp(stack_status_string(STACK_ERR_ALLOC), "STACK_ERR_ALLOC") == 0, "status string STACK_ERR_ALLOC");

    r = stack_destroy(&s);
    check(r.status == STACK_OK, "stack_destroy succeeds");
    check(s == NULL, "stack_destroy nulls the pointer");

    r = stack_destroy(NULL);
    check(r.status == STACK_ERR_NULL_STACK, "stack_destroy(NULL) returns STACK_ERR_NULL_STACK");

    printf("\n%d/%d tests passed\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
