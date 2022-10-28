#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "symtable.h"
#include "lexer.h"

int currentLine = 1;

char *stateToString(int state)
{
    switch (state)
    {
    case Start:
        return "Start";
    case Identifier:
        return "Identifier";
    case Var_prefix:
        return "Var_prefix";
    case Type_prefix:
        return "Type_prefix";
    case Start_opening:
        return "Start_opening";
    case End_closing:
        return "End_closing";
    case Assign:
        return "Assign";
    case Eq_in:
        return "Eq_in";
    case Eq_end:
        return "Eq_end";
    case Noteq_begin:
        return "Noteq_begin";
    case Noteq_in:
        return "Noteq_in";
    case Noteq_end:
        return "Noteq_end";
    case Concat:
        return "Concat";
    case Smaller:
        return "Smaller";
    case Larger:
        return "Larger";
    case Smaller_eq:
        return "Smaller_eq";
    case Larger_eq:
        return "Larger_eq";
    case Plus:
        return "Plus";
    case Minus:
        return "Minus";
    case Mul:
        return "Mul";
    case Div:
        return "Div";
    case Comment_l_in:
        return "Comment_l_in";
    case Comment_l_end:
        return "Comment_l_end";
    case Comment_b_in:
        return "Comment_b_in";
    case Comment_b_end_1:
        return "Comment_b_end_1";
    case Comment_b_end_2:
        return "Comment_b_end_2";
    case String_begin:
        return "String_begin";
    case String_in:
        return "String_in";
    case String_end:
        return "String_end";
    case String_esc:
        return "String_esc";
    case Int_in:
        return "Int_in";
    case Float_begin:
        return "Float_begin";
    case Float_in:
        return "Float_in";
    case Exp_begin:
        return "Exp_begin";
    case Exp_sign:
        return "Exp_sign";
    case Exp_in:
        return "Exp_in";
    case L_r_par:
        return "L_r_par";
    case R_r_par:
        return "R_r_par";
    case L_c_par:
        return "L_c_par";
    case R_c_par:
        return "R_c_par";
    case Whitespace:
        return "Whitespace";
    case Comma:
        return "Comma";
    case Colon:
        return "Colon";
    case Semicolon:
        return "Semicolon";
    case File_end:
        return "File_end";
    case ERROR:
        return "ERROR";
    case END:
        return "END";
    default:
        return "UNKNOWN";
    }
}

char *typeToString(token_type_t type)
{
    switch (type)
    {
    case T_Unknown:
        return "T_Unknown";
    case T_Identifier:
        return "T_Identifier";
    case T_Keyword:
        return "T_Keyword";
    case T_String:
        return "T_String";
    case T_Int:
        return "T_Int";
    case T_Float:
        return "T_Float";
    case T_Exp:
        return "T_Exp";
    case T_Block_comment:
        return "T_Block_comment";
    case T_Line_comment:
        return "T_Line_comment";
    case T_Var_prefix:
        return "T_Var_prefix";
    case T_Type_prefix:
        return "T_Type_prefix";
    case T_Assign:
        return "T_Assign";
    case T_Equal:
        return "T_Equal";
    case T_Not_equal:
        return "T_Not_equal";
    case T_Concat:
        return "T_Concat";
    case T_Smaller:
        return "T_Smaller";
    case T_Larger:
        return "T_Larger";
    case T_Smaller_eq:
        return "T_Smaller_eq";
    case T_Larger_eq:
        return "T_Larger_eq";
    case T_Plus:
        return "T_Plus";
    case T_Minus:
        return "T_Minus";
    case T_Mul:
        return "T_Mul";
    case T_Div:
        return "T_Div";
    case T_L_r_par:
        return "T_L_r_par";
    case T_R_r_par:
        return "T_R_r_par";
    case T_L_c_par:
        return "T_L_c_par";
    case T_R_c_par:
        return "T_R_c_par";
    case T_Comma:
        return "T_Comma";
    case T_Colon:
        return "T_Colon";
    case T_Semicolon:
        return "T_Semicolon";
    case T_Whitespace:
        return "T_Whitespace";
    case T_Start_opening:
        return "T_Start_opening";
    case T_End_closing:
        return "T_End_closing";
    case T_File_end:
        return "T_File_end";
    default:
        return "UNKNOWN";
    }
}

bool char_to_token(char c, token_t *token)
{
    if(c != EOF){
    token->lenght++;
    token->token = realloc(token->token, token->lenght*sizeof(char));
    if(token->token != NULL)
    {
        token->token[token->lenght-1] = c;
        return 1;
    }
    }
    return 0;
}

/// @brief
/// @param token
/// @return Error code
int getNextToken(token_t *token)
{
    // TODO: realoc when reached over 100 length
    token->lenght = 0;
    token->token = NULL;
    token->type = T_Unknown;
    fsm_state currentState = Start;
    fsm_state nextState;

    int c;
    do
    {
        c = getchar();
        switch (currentState)
        {
        case Start:
            if (isalpha(c) || c == '_')
                nextState = Identifier;
            else if (c == '$')
                nextState = Var_prefix;
            else if (c == '?')
                nextState = Type_prefix;
            else if (c == '"')
                nextState = String_begin;
            else if (isdigit(c))
                nextState = Int_in;
            else if (c == '!')
                nextState = Noteq_begin;
            else if (c == '.')
                nextState = Concat;
            else if (c == '<')
                nextState = Smaller;
            else if (c == '>')
                nextState = Larger;
            else if (c == '+')
                nextState = Plus;
            else if (c == '-')
                nextState = Minus;
            else if (c == '*')
                nextState = Mul;
            else if (c == '=')
                nextState = Assign;
            else if (c == '/')
                nextState = Div;
            else if (c == '(')
                nextState = L_r_par;
            else if (c == ')')
                nextState = R_r_par;
            else if (c == '{')
                nextState = L_c_par;
            else if (c == '}')
                nextState = R_c_par;
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
                nextState = Whitespace;
            else if (c == ',')
                nextState = Comma;
            else if (c == ':')
                nextState = Colon;
            else if (c == ';')
                nextState = Semicolon;
            else if (c == EOF)
                nextState = File_end;
            else
                nextState = ERROR;
            break;
        case Identifier:
            if (isalnum(c) || c == '_')
                nextState = Identifier;
            else
                token->type = T_Identifier;
            break;
        case Var_prefix:
            token->type = T_Var_prefix;
            break;
        case Type_prefix:
            if (c == '>')
                nextState = End_closing;
            else
                token->type = T_Type_prefix;
            break;
        case Assign:
            if (c == '=')
                nextState = Eq_in;
            else
                token->type = T_Assign;
            break;
        case Eq_in:
            if (c == '=')
                nextState = Eq_end;
            else
                nextState = ERROR;
            break;
        case Eq_end:
            token->type = T_Equal;
            break;
        case Noteq_begin:
            if (c == '=')
                nextState = Noteq_in;
            else
                nextState = ERROR;
            break;
        case Noteq_in:
            if (c == '=')
                nextState = Noteq_end;
            else
                nextState = ERROR;
            break;
        case Noteq_end:
            token->type = T_Not_equal;
            break;
        case Concat:
            token->type = T_Concat;
            break;
        case Smaller:
            if (c == '=')
                nextState = Smaller_eq;
            else if (c == '?')
                nextState = Start_opening;
            else
                token->type = T_Smaller;
            break;
        case Smaller_eq:
            token->type = T_Smaller_eq;
            break;
        case Larger:
            if (c == '=')
                nextState = Larger_eq;
            else
                token->type = T_Larger;
            break;
        case Larger_eq:
            token->type = T_Larger_eq;
            break;
        case Plus:
            token->type = T_Plus;

            break;
        case Minus:
            token->type = T_Minus;
            break;
        case Mul:
            token->type = T_Mul;
            break;
        case Div:
            if (c == '*')
                nextState = Comment_b_in;
            else if (c == '/')
                nextState = Comment_l_in;
            else
                token->type = T_Div;
            break;
        case Comment_l_in:
            if (c == '\n' || c == EOF)
                nextState = Comment_l_end;
            else
                nextState = Comment_l_in;
            break;
        case Comment_l_end:
            token->type = T_Line_comment;
            break;
        case Comment_b_in:
            if (c != '*')
                nextState = Comment_b_in;
            else if (c == '*')
                nextState = Comment_b_end_1;
            else
                nextState = ERROR;
            break;
        case Comment_b_end_1:
            if (c == '/')
                nextState = Comment_b_end_2;
            else if (c != '/')
                nextState = Comment_b_in;
            else
                nextState = ERROR;
            break;
        case Comment_b_end_2:
            token->type = T_Block_comment;
            break;
        case String_begin:
            if (c == '"')
                nextState = String_end;
            else if (c == '\\')
                nextState = String_esc;
            else if (c != '"' || c != '\\')
                nextState = String_in;
            else
                nextState = ERROR;
            break;
        case String_in:
            if (c == '"')
                nextState = String_end;
            else if (c == '\\')
                nextState = String_esc;
            else if (c != '"' || c != '\\')
                nextState = String_in;
            else
                nextState = ERROR;
            break;
        case String_end:
            token->type = T_String;
            break;
        case String_esc:
            nextState = String_in;
            break;
        case Int_in:
            if (isdigit(c))
                nextState = Int_in;
            else if (c == '.')
                nextState = Float_begin;
            else if (c == 'e' || c == 'E')
                nextState = Exp_begin;
            else
                token->type = T_Int;
            break;
        case Float_begin:
            if (isdigit(c))
                nextState = Float_in;
            else
                nextState = ERROR;
            break;
        case Float_in:
            if (isdigit(c))
                nextState = Float_in;
            else if (c == 'e' || c == 'E')
                nextState = Exp_begin;
            else
                token->type = T_Float;
            break;
        case Exp_begin:
            if (c == '+' || c == '-')
                nextState = Exp_sign;
            else if (isdigit(c))
                nextState = Exp_in;
            else
                nextState = ERROR;
            break;
        case Exp_sign:
            if (isdigit(c))
                nextState = Exp_in;
            else
                nextState = ERROR;
            break;
        case Exp_in:
            if (isdigit(c))
                nextState = Exp_in;
            else
                token->type = T_Exp;
            break;
        case Whitespace:
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
                nextState = Whitespace;
            else
                token->type = T_Whitespace;
            break;
        case L_r_par:
            token->type = T_L_r_par;
            break;
        case R_r_par:
            token->type = T_R_r_par;
            break;
        case L_c_par:
            token->type = T_L_c_par;
            break;
        case R_c_par:
            token->type = T_R_c_par;
            break;
        case Comma:
            token->type = T_Comma;
            break;
        case Colon:
            token->type = T_Colon;
            break;
        case Semicolon:
            token->type = T_Semicolon;
            break;
        case Start_opening:
            token->type = T_Start_opening;

            break;
        case End_closing:
            token->type = T_End_closing;
            break;
        case File_end:
            token->type = T_File_end;
            break;
        default:
            token->type = T_Unknown;
            nextState = Start;
            break;
        }

        if (token->type != T_Unknown)
        {
            char_to_token('\0',token);
            token->line = currentLine;
            ungetc(c, stdin);
            return 0;
        }
        else if (nextState == ERROR)
        {
            printf("err\n");
            char_to_token('\0',token);
            ungetc(c, stdin);
            return 1;
        }
        if (c == '\n')
        {
            currentLine++;
        }

        // printf("Current state: %s -> Char %c (%d) -> Next state: %s\n", stateToString(currentState), c, c, stateToString(nextState));
        currentState = nextState;
        char_to_token(c,token);
    } while (token->type == T_Unknown);
    return 0;
}

int main()
{
    // Debug function, lol
    token_t *tokens[1000];

    for (int i = 0; i < 1000; i++)
    {
        tokens[i] = malloc(sizeof(token_t));
    }
    // Go through tokens until End of file token type
    int i = 0;
    while (true)
    {
        getNextToken(tokens[i]);
        if (tokens[i]->type != T_Whitespace)
        {
            printf("%d)\t%s", currentLine, tokens[i]->token);
            if (strlen(tokens[i]->token) < 8)
                printf("\t");
            printf("\t%s\n", typeToString(tokens[i]->type));
        }
        if (tokens[i]->type == T_File_end)
        {
            i++;
            break;
        }
        i++;
    }
    printf("\nImprovised symbol table:\n");
    hash_table_t *table = hash_table_init(100);
    for (int j = 0; j < i; j++)
    {
            double lf = (double)table->count / table->size;
            if(lf > 0.6)
            {
                //int nsize = table->size * 2;
                table = resize(table);
            }
            hash_table_insert(table, token_to_symbol(tokens[j]));
    }
    hash_table_lookup(table, "strict_types");
    hash_table_print(table);
    for (int i = 0; i < 1000; i++)
    {
        free(tokens[i]);
    }
    hash_table_free(table);
    return 0;
}