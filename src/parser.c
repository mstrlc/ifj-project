#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"


//KONEC BRACKET ZASOBNIKU

int prog (token_t *token, b_stack* stack){

    ERR_CODE error_code = (ERR_CODE) malloc(sizeof(ERR_CODE));
    switch(token -> type){
        case T_Var_id:
            getNextToken(token);
          
            if(token -> type == T_Assign){
                getNextToken(token);

                if(token -> type == T_String){
                    getNextToken(token);
                    if(token -> type == T_Semicolon)
                    {
                        //CODE GEN
                        error_code = pass;
                    }
                    else{
                        return 1;
                    }
                }

                else if(token -> type == T_Int){
                    getNextToken(token);

                    if(token -> type == T_Semicolon)
                    {
                        //CODE GEN
                        error_code = pass;
                    }
                    else{
                        return 1;
                    }
                }

                else if(token -> type == T_Float){
                    getNextToken(token);
                    if(token -> type == T_Semicolon)
                    {
                        //CODE GEN
                        error_code = pass;
                    }
                    else{
                        return 1;
                    }
                }

                else if(token -> type == T_Exp){
                    getNextToken(token);
                    if(token -> type == T_Semicolon)
                    {
                        //CODE GEN
                        error_code = pass;
                    }
                    else{
                        return 1;
                    }   
                }

                else{
                    //check_expression();
                    error_code = parser_err;
                    return 1;
                }
            } // if(token -> type == T_Equal)

            else if(token -> type == T_Semicolon){
                    //DEFVAR CODE GEN
            }
            
            else{
                return 1;
            }

        break; //case T_Var_id
        
        case T_Keyword_If:
            getNextToken(token);
            int max_expression_lenght = 100;
            int exp_count = 0;
            if (token -> type == T_L_r_par){
                do {
                    if(exp_count < max_expression_lenght){
                        exp_count++;
                        getNextToken(token);
                        // udelej string ktery pak narvy do check_expression()
                    }
                    else 
                        return 1;
                } while (token -> type != T_R_r_par);

                getNextToken(token);
                if(token -> type == T_L_c_par){
                    //vytvori se nejaky label, pak se printne JUMP label a ten stejne pojmenovany label se printe az se narazi na '}' 
                    b_stack_push(stack, "LABEL");
                }
            }
        break;
        
        case T_R_c_par:
            //popni stack, podivej se jestli je pred tim T_L_r_par a kdyz nebude vyhod chybu
            if(!b_stack_is_empty(stack)){
                b_stack_pop(stack); //zasobnik bude popovat labely in the future
                return 0;
            }
            else
                return 1;
        break;

        //tohle realne osetrit v lexeru nebo mainu na tokens[0]
        case T_Start_opening:
            getNextToken(token);

            if (token -> type == T_Identifier && strcmp(token -> data, "php") == 0)
            {
                return 0;
            }
            return 0;
        break;

        case T_Line_comment:
            return 0;
        break;

        case T_Block_comment:
            return 0;
        break;

        case T_File_end:
            if(!b_stack_is_empty(stack)){ // jestli je otevrena zavorka ale neni zadna ukoncena
                return 1;
            }
            return 0;
        break;

        default:
            return 1;
        break;
    }
    return 0;
}
