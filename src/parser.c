#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"

int prog (token_t *token, b_stack* stack){

    ERR_CODE error_code = (ERR_CODE) malloc(sizeof(ERR_CODE)); // zatim nikde nevyuzivam

    switch(token -> type){
        case T_Var_id:
            getNextToken(token);
          
            if(token -> type == T_Assign){
                getNextToken(token);

                if(token -> type == T_Var_id){ 
                    getNextToken(token);
    
                    if(token -> type == T_Semicolon) // tyhle konstrukce nebudou brat vicenasobne prirazeni jako $var1 = $var2 = 8;
                    {   
                        //CODE GEN
                        error_code = pass;
                        return 0;
                    }
                    else{
                        return 1;
                    }
                }

                if(token -> type == T_String){
                    getNextToken(token);
                    if(token -> type == T_Semicolon)
                    {
                        //CODE GEN
                        return 0;
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
                        return 0;
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
                        return 0;
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
                        return 0;
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
            int max_expression_lenght = 100; // nemuzeme jinak overit, ze po otevrene zavorce nasleduje uzavrena
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

                    b_stack_elem* elem = (b_stack_elem*)malloc(sizeof(struct b_stack_elem));  
                    elem -> type = type_if;
                    strcpy(elem ->label, "LABELL");
                    b_stack_push(stack, elem); // nefunguje pushovani stringu actually se pushne jen NULL ale do code genu neni potreba fixovat

                    return 0;
                }
            }
        break;
        
        case T_R_c_par:
            //popni stack, zjisti co za keyword jsme zrovna ukoncili a vypis label

            if(!b_stack_is_empty(stack)){            
                b_stack_elem* elem = b_stack_pop(stack); 

                if(elem -> type == type_if){
                    getNextToken(token);
                    if(token -> type == T_Keyword_Else){
                        //code gen asi o.o
                        getNextToken(token);
                        if(token -> type == T_L_c_par){
                            b_stack_elem* elem = (b_stack_elem*)malloc(sizeof(struct b_stack_elem));  
                            elem -> type = type_else;
                            b_stack_push(stack, elem);
                            return 0;
                        }
                        else{
                            return 1;
                        }
                        
                    }
                    if(elem -> type == type_else_if){

                    }
                    if(elem -> type == type_else){

                    }
                }
                return 0;
            }
            else //if(!b_stack_is_empty(stack) v zasobniku nejsou zadne { tudiz neni pripustny znak }
                return 1;
        break;

        //tohle realne osetrit v lexeru nebo mainu na tokens[0], protoze tohle nekontroluje jestli je to prvni token a ani to nema parser jak urcit
        case T_Start_opening:
            getNextToken(token);
            if (token -> type == T_Identifier && strcmp(token -> data, "php") == 0)
            {
                return 0;
            }
             return 1;
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
