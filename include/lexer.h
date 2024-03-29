/**
 * @file lexer.h
 *
 * Implementation of lexical analysis and working with tokens
 *
 * IFJ project 2022
 *
 * @author <xstrel03> Matyáš Strelec
 *
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

/**
 * @brief States of FSM
 *
 */
typedef enum fsm_state_t
{
    Start,
    Identifier,
    Var_id_prefix,
    Var_id,
    Type_prefix,
    End_closing,
    Start_opening,
    Assign,
    Eq_in,
    Eq_end,
    Noteq_begin,
    Noteq_in,
    Noteq_end,
    Concat,
    Smaller,
    Larger,
    Smaller_eq,
    Larger_eq,
    Plus,
    Minus,
    Mul,
    Div,
    Comment_l_in,
    Comment_l_end,
    Comment_b_in,
    Comment_b_end_1,
    Comment_b_end_2,
    String_begin,
    String_in,
    String_end,
    String_esc,
    Int_in,
    Float_begin,
    Float_in,
    Exp_begin,
    Exp_sign,
    Exp_in,
    L_r_par,
    R_r_par,
    L_c_par,
    R_c_par,
    Whitespace,
    Comma,
    Colon,
    Semicolon,
    File_end,
    ERROR
} fsm_state_t;

/**
 * @brief Types of tokens
 *
 */
typedef enum token_type_t
{
    T_Unknown,
    T_Identifier,
    T_Var_id,
    T_Keyword,
    T_String,
    T_Int,
    T_Float,
    T_Block_comment,
    T_Line_comment,
    T_Var_prefix,
    T_Type_prefix,
    T_Assign,
    T_Equal,
    T_Not_equal,
    T_Concat,
    T_Smaller,
    T_Smaller_eq,
    T_Larger,
    T_Larger_eq,
    T_Plus,
    T_Minus,
    T_Mul,
    T_Div,
    T_Whitespace,
    T_L_r_par,
    T_R_r_par,
    T_L_c_par,
    T_R_c_par,
    T_Comma,
    T_Colon,
    T_Semicolon,
    T_Start_opening,
    T_End_closing,
    T_File_end,
    T_Keyword_Else,
    T_Keyword_Float,
    T_Keyword_Function,
    T_Keyword_If,
    T_Keyword_Int,
    T_Keyword_Null,
    T_Keyword_Return,
    T_Keyword_String,
    T_Keyword_Void,
    T_Keyword_While,
    T_Error
} token_type_t;

/**
 * @brief Token structure
 *
 */
typedef struct token_t
{
    token_type_t type;
    char *data;
    int line;
    size_t length;
    struct token_t *prev;
    struct token_t *next;
} token_t;

/**
 * @brief Token list structure
 *
 */
typedef struct token_list_t
{
    token_t *firstToken;
    token_t *activeToken;
    token_t *lastToken;

} token_list_t;

void printToken(token_t *token);

void printTokenList(token_list_t *list);

void initTokenList(token_list_t *List);

int fillTokenList(token_list_t *list);

void freeTokenList(token_list_t *list);

token_t *TopToken(token_list_t *list);

int PopToken(token_list_t *list);

int PushToken(token_list_t *list, token_t *token);

#endif
