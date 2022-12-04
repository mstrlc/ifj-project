#include "../include/stack.h"

stack* stack_init(){
    stack* s = (stack*)malloc(sizeof(struct stack));
    s -> top = 0;
    return s;
}

void stack_push(stack* s, char* defvar, stack* defvar_stack){
    if (s->top < BRACKET_STACK_MAX ) {
        s->top++;
        stack_elem* elem = (stack_elem*)malloc(sizeof(stack_elem));
        elem -> defvar = defvar;
        elem -> defvar_stack = defvar_stack;
        s->dataArray[s->top] = elem;
    }
}

void stack_pop(stack* s){
    s -> top--;
}

stack_elem* stack_top(stack* s){
    return s -> dataArray[s->top];
}

bool stack_is_empty(stack* s){
    return (s->top == 0);
}

void print_and_pop(stack* s){
    while(!stack_is_empty(s)){
        printf("%s\n", stack_top(s)->defvar);
        stack_pop(s);
    }
}

// v prvnim pruchodu dostaneme defvars obracene - liny nato implementovat queue 
stack* stack_reverse(stack* s){
    stack* revStack = stack_init();
    while(!stack_is_empty(s)){
        stack_push(revStack, stack_top(s)->defvar, stack_top(s)->defvar_stack);
        stack_pop(s);
    }
    return revStack;
}
