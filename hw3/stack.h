#ifndef STACK_H
#define STACK_H

#include <stddef.h> 
#include <stdint.h>   


#define STACK_MIN_CAPACITY 8u   
#define STACK_GROW_FACTOR  2u   


typedef enum {
    STACK_OK,               
    STACK_ERR_NULL_STACK,   
    STACK_ERR_NULL_STRING,  
    STACK_ERR_EMPTY,        
    STACK_ERR_ALLOC,        
} StackStatus;

typedef struct {
    StackStatus  status;  
    char        *value;  
} StackResponse;


typedef struct Stack Stack;


StackResponse stack_create(Stack **out_stack);

StackResponse stack_destroy(Stack **stack_ptr);

StackResponse stack_push(Stack *stack, const char *str);

StackResponse stack_pop(Stack *stack);

StackResponse stack_peek(const Stack *stack);

StackResponse stack_size(const Stack *stack);
#define STACK_SIZE_VAL(r) ((size_t)(uintptr_t)(r).value)

StackResponse stack_is_empty(const Stack *stack);

void stack_response_free_value(StackResponse *r);

const char *stack_status_string(StackStatus status);

#endif
