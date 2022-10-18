#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


char *stateToString(int state)
{
    switch(state)
    {
        case Start: return "Start";
        case Identifier: return "Identifier";
        case Var_prefix: return "Var_prefix";
        case Type_prefix: return "Type_prefix";
        case End_closing: return "End_closing";
        case Assign: return "Assign";
        case Eq_in: return "Eq_in";
        case Eq_end: return "Eq_end";
        case Noteq_begin: return "Noteq_begin";
        case Noteq_in: return "Noteq_in";
        case Noteq_end: return "Noteq_end";
        case Concat: return "Concat";
        case Smaller: return "Smaller";
        case Larger: return "Larger";
        case Smaller_eq: return "Smaller_eq";
        case Larger_eq: return "Larger_eq";
        case Plus: return "Plus";
        case Minus: return "Minus";
        case Mul: return "Mul";
        case Div: return "Div";
        case Comment_l_in: return "Comment_l_in";
        case Comment_l_end: return "Comment_l_end";
        case Comment_b_in: return "Comment_b_in";
        case Comment_b_end_1: return "Comment_b_end_1";
        case Comment_b_end_2: return "Comment_b_end_2";
        case String_begin: return "String_begin";
        case String_in: return "String_in";
        case String_end: return "String_end";
        case String_esc: return "String_esc";
        case Int_in: return "Int_in";
        case Float_begin: return "Float_begin";
        case Float_in: return "Float_in";
        case Exp_begin: return "Exp_begin";
        case Exp_sign: return "Exp_sign";
        case Exp_in: return "Exp_in";
        case L_r_par: return "L_r_par";
        case R_r_par: return "R_r_par";
        case L_c_par: return "L_c_par";
        case R_c_par: return "R_c_par";
        case Whitespace: return "Whitespace";
        case Comma: return "Comma";
        case Colon: return "Colon";
        case Semicolon: return "Semicolon";
        case ERROR: return "ERROR";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

int main()
{
    FSM_State currentState = Start;
    FSM_State nextState;

    char string[1000];
    int pos = 0;

    int c;
    while ((c = getchar()) != EOF)
    {
        nextState = Start;

        string[pos] = c;
        pos++;

        {
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
                else if (c == ' ' || c == '\t' || c == '\n')
                    nextState = Whitespace;
                else if (c == ',')
                    nextState = Comma;
                else if (c == ':')
                    nextState = Colon;
                else if (c == ';')
                    nextState = Semicolon;
                else
                    nextState = ERROR;
                break;
            case Identifier:
                if (isalnum(c) || c == '_')
                    nextState = Identifier;
                else
                    nextState = END;
                break;
            case Var_prefix:
                nextState = END;
                break;
            case Type_prefix:
                if (c == '>')
                    nextState = End_closing;
                else
                    nextState = END;
            case Assign:
                if (c == '=')
                    nextState = Eq_in;
                else
                    nextState = END;
                break;
            case Eq_in:
                if (c == '=')
                    nextState = Eq_end;
                else
                    nextState = ERROR;
                break;
            case Eq_end:
                nextState = END;
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
                nextState = END;
                break;
            case Concat:
                nextState = END;
                break;
            case Smaller:
                if (c == '=')
                    nextState = Smaller_eq;
                else
                    nextState = END;
                break;
            case Smaller_eq:
                nextState = END;
                break;
            case Larger:
                if (c == '=')
                    nextState = Larger_eq;
                else
                    nextState = END;
                break;
            case Larger_eq:
                nextState = END;
                break;
            case Plus:
                nextState = END;
                break;
            case Minus:
                nextState = END;
                break;
            case Mul:
                nextState = END;
                break;
            case Div:
                if (c == '*')
                    nextState = Comment_b_in;
                else if (c == '/')
                    nextState = Comment_l_in;
                else
                    nextState = END;
                break;
            case Comment_l_in:
                if (c != '\n' || c != EOF)
                    nextState = Comment_l_in;
                else if (c == '\n' || c == EOF)
                    nextState = Comment_l_end;
                else
                    nextState = ERROR;
                break;
            case Comment_l_end:
                nextState = END;
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
                nextState = END;
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
                nextState = END;
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
                    nextState = END;
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
                    nextState = END;
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
                    nextState = END;
                break;
            case Whitespace:
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
                    nextState = Whitespace;
                else
                    nextState = END;
                break;
            case L_r_par:
                nextState = END;
                break;
            case R_r_par:
                nextState = END;
                break;
            case L_c_par:
                nextState = END;
                break;
            case R_c_par:
                nextState = END;
                break;
            case Comma:
                nextState = END;
                break;
            case Colon:
                nextState = END;
                break;
            case Semicolon:
                nextState = END;
                break;
            case End_closing:
                nextState = END;
                break;
            }
        }
  
        // printf("\t[%s]\t'%c'\n",stateToString(currentState), c);
  
        if (nextState == END)
        {
            string[pos - 1] = '\0';
            if(currentState != Whitespace)
                printf("%s\t[%s]\n", string, stateToString(currentState));
            pos = 0;
            currentState = Start;
            ungetc(c, stdin);
        }
        else
        {
            currentState = nextState;
        }
    }
}

// ! TODO newline doesnt count as whitespace
// ! TODO : colon operator token