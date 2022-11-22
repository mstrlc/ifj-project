#include "../include/stack.h"

void b_stack_init(b_stack* s){
    s -> top = 0;
}

void b_stack_push(b_stack* s, b_stack_elem* elem){
    if (s->top < BRACKET_STACK_MAX ) {
        s->top++;
        s->dataArray[s->top] = elem;
    }
}

void b_stack_pop(b_stack* s){
    s -> top--;
}

b_stack_elem* b_stack_top(b_stack* s){
    return s -> dataArray[s->top];
}

bool b_stack_is_empty(b_stack* s){
    return (s->top == 0);
}