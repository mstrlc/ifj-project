#include "../include/stack.h"

void b_stack_init(b_stack* s){
    s -> top = 0;
}

void b_stack_push(b_stack* s, char* label){
    if (s->top < BRACKET_STACK_MAX) {
        s->dataArray[s->top] = label;
        s->top++;
    }
}
char* b_stack_pop(b_stack* s){
    char* data = s -> dataArray[s->top];
    s -> top--;
    return data;
}

bool b_stack_is_empty(b_stack* s){
    return (s->top == 0);
}