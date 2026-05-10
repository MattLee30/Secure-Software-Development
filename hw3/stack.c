#define _POSIX_C_SOURCE 200809L

#include "stack.h"

#include <stdlib.h>   
#include <string.h>  

struct Stack {
    char   **items;    
    size_t   size;     
    size_t   capacity; 
};

static StackResponse make_status(StackStatus status)
{
    StackResponse r;
    r.status = status;
    r.value  = NULL;
    return r;
}

static StackResponse make_value(char *value)
{
    StackResponse r;
    r.status = STACK_OK;
    r.value  = value;
    return r;
}

static StackStatus resize(Stack *stack, size_t new_capacity)
{
    if (new_capacity < STACK_MIN_CAPACITY) {
        new_capacity = STACK_MIN_CAPACITY;
    }

    if (new_capacity < stack->size) {
        return STACK_ERR_ALLOC; 
    }

    char **new_items = realloc(stack->items, new_capacity * sizeof(char *));
    if (new_items == NULL) {
        return STACK_ERR_ALLOC;
    }

    stack->items    = new_items;
    stack->capacity = new_capacity;
    return STACK_OK;
}


StackResponse stack_create(Stack **out_stack)
{
    if (out_stack == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }

    *out_stack = NULL;

    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) {
        return make_status(STACK_ERR_ALLOC);
    }

    s->items = malloc(STACK_MIN_CAPACITY * sizeof(char *));
    if (s->items == NULL) {
        free(s);
        return make_status(STACK_ERR_ALLOC);
    }

    s->size     = 0;
    s->capacity = STACK_MIN_CAPACITY;

    *out_stack = s;
    return make_status(STACK_OK);
}

StackResponse stack_destroy(Stack **stack_ptr)
{
    if (stack_ptr == NULL || *stack_ptr == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }

    Stack *s = *stack_ptr;

    for (size_t i = 0; i < s->size; i++) {
        free(s->items[i]);
        s->items[i] = NULL; 
    }

    free(s->items);
    s->items = NULL;

    free(s);

    *stack_ptr = NULL;

    return make_status(STACK_OK);
}


StackResponse stack_push(Stack *stack, const char *str)
{
    if (stack == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }
    if (str == NULL) {
        return make_status(STACK_ERR_NULL_STRING);
    }

    if (stack->size == stack->capacity) {
        StackStatus st = resize(stack, stack->capacity * STACK_GROW_FACTOR);
        if (st != STACK_OK) {
            return make_status(st);
        }
    }

    char *copy = strdup(str);
    if (copy == NULL) {
        return make_status(STACK_ERR_ALLOC);
    }

    stack->items[stack->size++] = copy;
    return make_status(STACK_OK);
}

StackResponse stack_pop(Stack *stack)
{
    if (stack == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }
    if (stack->size == 0) {
        return make_status(STACK_ERR_EMPTY);
    }

    char *caller_copy = strdup(stack->items[stack->size - 1]);
    if (caller_copy == NULL) {
        return make_status(STACK_ERR_ALLOC);
    }

    free(stack->items[stack->size - 1]);
    stack->items[stack->size - 1] = NULL; 
    stack->size--;

    if (stack->capacity > STACK_MIN_CAPACITY &&
        stack->size <= stack->capacity / 4) {
        resize(stack, stack->capacity / STACK_GROW_FACTOR);
    }

    return make_value(caller_copy);
}

StackResponse stack_peek(const Stack *stack)
{
    if (stack == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }
    if (stack->size == 0) {
        return make_status(STACK_ERR_EMPTY);
    }

    char *caller_copy = strdup(stack->items[stack->size - 1]);
    if (caller_copy == NULL) {
        return make_status(STACK_ERR_ALLOC);
    }

    return make_value(caller_copy);
}


StackResponse stack_size(const Stack *stack)
{
    if (stack == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }

    StackResponse r;
    r.status = STACK_OK;
    r.value  = (char *)(uintptr_t)stack->size;
    return r;
}

StackResponse stack_is_empty(const Stack *stack)
{
    if (stack == NULL) {
        return make_status(STACK_ERR_NULL_STACK);
    }

    StackResponse r;
    r.status = STACK_OK;
    r.value  = (stack->size == 0) ? NULL : (char *)1;
    return r;
}

void stack_response_free_value(StackResponse *r)
{
    if (r == NULL) {
        return;
    }
    free(r->value);
    r->value = NULL;
}

const char *stack_status_string(StackStatus status)
{
    switch (status) {
        case STACK_OK:              return "STACK_OK";
        case STACK_ERR_NULL_STACK:  return "STACK_ERR_NULL_STACK";
        case STACK_ERR_NULL_STRING: return "STACK_ERR_NULL_STRING";
        case STACK_ERR_EMPTY:       return "STACK_ERR_EMPTY";
        case STACK_ERR_ALLOC:       return "STACK_ERR_ALLOC";
        default:                    return "STACK_ERR_UNKNOWN";
    }
}
