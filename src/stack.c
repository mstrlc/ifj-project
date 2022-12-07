/**
 * @file stack.c
 *
 * Implementation of stack used in parser
 *
 * IFJ project 2022
 *
 * @author <xklon00> Dominik Klon
 *
 */

#include "../include/stack.h"

/**
 * @brief Stack initialization
 *
 *
 * @return stack* Pointer to a stack structure
 *
 */
stack *stack_init()
{
    stack *s = (stack *)malloc(sizeof(struct stack));
    s->top = 0;
    return s;
}

/**
 * @brief Stack dispose
 *
 * Free all the stack elements.
 *
 * @param stack* Pointer to a stack structure
 *
 */
void stack_dispose(stack *s)
{
    while (!stack_is_empty(s))
    {
        free(stack_top(s));
        stack_pop(s);
    }
    free(s);
}

/**
 * @brief Stack push
 *
 * Push string to a selected stack
 *
 * @param stack* Pointer to a stack structure
 * @param char* String to be pushed
 */
void stack_push(stack *s, char *stack_str)
{
    if (s->top < BRACKET_STACK_MAX)
    {
        s->top++;
        stack_elem *elem = (stack_elem *)malloc(sizeof(stack_elem));
        elem->stack_str = stack_str;
        s->dataArray[s->top] = elem;
    }
}

/**
 * @brief Stack pop
 *
 * Pop a stack (without returning a value)
 *
 * @param stack* Pointer to a stack structure
 *
 */
void stack_pop(stack *s)
{
    s->top--;
}

/**
 * @brief Stack top
 * Return value at the top.
 *
 * @param stack* Pointer to a stack structure
 * @return stack_elem* Pointer to a stack element
 *
 */
stack_elem *stack_top(stack *s)
{
    return s->dataArray[s->top];
}

/**
 * @brief Check if stack is empty
 *
 * @param stack* Pointer to a stack structure
 * @return bool True when stack is empty, false when it is not empty
 *
 */
bool stack_is_empty(stack *s)
{
    return (s->top == 0);
}

/**
 * @brief Print all values from a stack
 *
 * @param stack* Pointer to a stack structure
 *
 */
void print_and_pop(stack *s)
{
    while (!stack_is_empty(s))
    {
        printf("%s\n", stack_top(s)->stack_str);
        stack_pop(s);
    }
}
