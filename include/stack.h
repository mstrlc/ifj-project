#ifndef STACK
#define STACK
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BRACKET_STACK_MAX 1000

// zasobnik na spravne checkovani uzaviracim slozenych zavorek .. pokud neni nic v zasobniku
// znameno to ze pred tim nebyla zadna oteviraci zavorka a tudiz zpusobi chybi
// pozdej v nem bude asi vic informaci nebo tu bude i vic zasobniku na vic druhu informaci treba scopy prommenych funkci atd


typedef struct stack_elem{
    char* defvar;
    struct stack* defvar_stack;
} stack_elem;

typedef struct stack{
    stack_elem* dataArray[BRACKET_STACK_MAX];
    int top;
} stack;

stack* stack_init();

void stack_push(stack* s, char* defvar, stack* defvar_stack);

void  stack_pop(stack* s);

stack_elem* stack_top(stack* s);

bool stack_is_empty(stack* s);

void print_and_pop(stack* s);

stack* stack_reverse(stack* s);

#endif
