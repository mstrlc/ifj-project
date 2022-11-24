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

typedef enum bracket_body_type{ // body type ( ͡° ͜ʖ ͡°)
    type_if,
    type_else,
    type_else_if,
    type_while,
    type_function
} bracket_body_type;

typedef struct b_stack_elem{
    char label[50];
    bracket_body_type type;
    
} b_stack_elem;

typedef struct b_stack{
    b_stack_elem* dataArray[BRACKET_STACK_MAX];
    int top;
} b_stack;

void b_stack_init(b_stack* s);

void b_stack_push(b_stack* s, b_stack_elem * elem);

void  b_stack_pop(b_stack* s);

b_stack_elem* b_stack_top(b_stack* s);

bool b_stack_is_empty(b_stack* s);

#endif
