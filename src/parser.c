#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parser.h"


//0... PASS
//1... syntax err

#define CREATE_ASSIGN(tType)                       \
        strcat(lineToPrint, " ");           \
        strcat(lineToPrint, tType);           \
        strcat(lineToPrint, token->data);   \
        getNextToken(token);                \
        if(token -> type == T_Semicolon)    \
        {                                   \
            printf("MOVE TF@%s \n", lineToPrint); \
            return 0;                       \
        }                                   \
        else{                               \
            return 1;                       \
        }                                   \

int prog (token_t *token, b_stack* stack){
char lineToPrint[100];

    switch(token -> type){
        case T_Var_id:
            printf("DEFVAR TF@%s \n", token->data); // az se bude pracovat s tabulkou symbolu, pridej kontrolu, jestli je promenna definovana

            strcpy(lineToPrint, token -> data);
            getNextToken(token);
          
            if(token -> type == T_Assign){ // cely tenhle kod v T_Assign bude potreba nahradit expression parserem, tohle je tady jen for fun abych neco aspon generoval
                getNextToken(token);
                if(token -> type == T_Var_id){ 
                    CREATE_ASSIGN("TF@");
                }
                
                else if(token -> type == T_String){
                    CREATE_ASSIGN("string@");
                }

                else if(token -> type == T_Int){
                    CREATE_ASSIGN("int@");
                }

                else if(token -> type == T_Float){
                    CREATE_ASSIGN("float@");
                }

                else if(token -> type == T_Exp){
                    getNextToken(token);
                    return 0;
                   
                }

                else if(token -> type == T_Semicolon){ //semicolon asi taky nechame, to by nemel delat exp parser
                    printf("DEFVAR %s \n", token->data);
                }

                else if(token -> type == T_Identifier){         // prirazeni fuknce nechame uvidime jak to vymyslime s express parserem
                    getNextToken(token);
                    if(token -> type != T_L_r_par)
                        return 1;

                    if(check_call_args(token) != 0){
                        return check_call_args(token);
                    }

                    getNextToken(token);
                    if(token -> type != T_Semicolon)
                        return 1;
                }

                else{
                    //check_expression(); // idk kde se to bude checkovat
                    //_code = parser_err;
                    return 1;
                }
            } // if(token -> type == T_Equal)
        break; //case T_Var_id
        
        case T_Keyword_If:
            getNextToken(token);
            if (token -> type != T_L_r_par)
                return 1;

            int max_expression_lenght = 100; // nemuzeme jinak overit, ze po otevrene zavorce nasleduje uzavrena
            int exp_count = 0;

            do {
                exp_count++;
                if(exp_count > max_expression_lenght)
                    return 1;

                getNextToken(token);
                // udelej string ktery pak narvy do check_expression()

            } while (token -> type != T_R_r_par);

            getNextToken(token);
            if(token -> type != T_L_c_par)
                return 1;

            //vytvori se nejaky label, pak se printne JUMP label a ten stejne pojmenovany label se printe az se narazi na '}' 
            b_stack_elem* elem = (b_stack_elem*)malloc(sizeof(struct b_stack_elem));  
            elem -> type = type_if;
            strcpy(elem ->label, make_random_string());

            b_stack_push(stack, elem);

            // printf("PUSHFRAME\n");
            // printf("CREATEFRAME\n");
            printf("JUMPIFNEQ %s LF@vysledekExp bool@true\n",elem->label); // tady bude nejake printovani podminene na vyrazu
        break;
        
        case T_R_c_par:
            //popni stack, zjisti co za keyword { jsme zrovna ukoncili a vypis label
            if(b_stack_is_empty(stack))
                return 1;
                     
            elem = b_stack_top(stack);
            

            if(elem -> type == type_if){            // kdyz ukoncujeme telo IFu

                getNextToken(token);

                if(token -> type == T_Keyword_Else){
                    getNextToken(token);

                    if(token -> type != T_L_c_par)
                        return 1;

                    char* tempIfLabel = elem->label;            // potrebujeme si uchovat label ifu, abysme

                    b_stack_elem* elem = (b_stack_elem*)malloc(sizeof(struct b_stack_elem));  //vygenerujeme novy jump pro else, abysme mohli skocit na konec else jestlize v if bude true
                    elem -> type = type_else;
                    strcpy(elem -> label, make_random_string());
                    
                    printf("JUMP %s\n",elem -> label);
                    printf("LABEL %s\n", tempIfLabel);
                    b_stack_pop(stack);
                        
                    // printf("PUSHFRAME\n");
                    // printf("CREATEFRAME\n");

                    b_stack_push(stack, elem);
                }
                else{
                    // printf("POPFRAME\n");
                    printf("LABEL %s\n", elem ->label);
                    b_stack_pop(stack);
                    return prog(token, stack); // bud muze nasledovat za if else, nebo statement-list
                }    
            }

            if(elem -> type == type_else){
                
                elem = b_stack_top(stack);
                b_stack_pop(stack);

                // printf("POPFRAME\n");
                printf("LABEL %s\n", elem ->label);
                
                // b_stack_pop(stack);
            }
            
            if(elem -> type == type_while){
                b_stack_pop(stack);
                // b_stack_pop(stack);
            }

            if(elem -> type == type_function){
                b_stack_pop(stack);

            }
        break;

        case T_Keyword_While: // od ted to zacinam psat jak clovek
            getNextToken(token);

            if(token -> type != T_L_r_par)
                return 1;

            max_expression_lenght = 100; // nemuzeme jinak overit, ze po otevrene zavorce nasleduje uzavrena
            exp_count = 0;

            do{
                exp_count++;
                getNextToken(token);
                if (exp_count > max_expression_lenght)
                    return 1;

                //nacti expresiion a posli ho do zpracovani 
            } while (token -> type != T_R_r_par);

            getNextToken(token);
            if(token -> type != T_L_c_par)
                return 1;
            
            elem = (b_stack_elem*)malloc(sizeof(struct b_stack_elem));  
            elem -> type = type_while;
            strcpy(elem -> label, "WHILEE");

            b_stack_push(stack, elem);

        break;

        case T_Keyword_Function:
            getNextToken(token);

            if(token -> type != T_Identifier)
                return 0;

            getNextToken(token);
            if(token -> type != T_L_r_par)
                return 1;

            if(check_args(token) != 0){
                return check_args(token);
            }

            getNextToken(token);
            if(token->type != T_Colon)
                return 1;

            getNextToken(token);
            if(token -> type == T_Keyword_String){

            }  
            else if(token -> type == T_Keyword_Int){

            }
            else if(token->type == T_Keyword_Float){

            }
            else{
                return 1;
            }

            getNextToken(token);
            if(token -> type != T_L_c_par)
                return 1;
            
            elem = (b_stack_elem*)malloc(sizeof(struct b_stack_elem));  
            elem -> type = type_function;
            strcpy(elem ->label, "FUNCTION");
            
            b_stack_push(stack, elem);
            
            // if(token -> type != T_L_c_par)
            //     return 1;

        break;

        //tohle realne osetrit v lexeru nebo mainu na tokens[0], protoze tohle nekontroluje jestli je to prvni token a ani to nema parser jak urcit
        case T_Start_opening:
            getNextToken(token);

            if (strcmp(token -> data, "php") != 0)
                return 1;

        break;

        case T_Line_comment:
            //
        break;

        case T_Block_comment:
            //
        break;

        case T_File_end: 
            if(!b_stack_is_empty(stack)) // jestli je otevrena zavorka ale neni zadna ukoncena
                return 1;
        break;

        default:
            return 1;
        break;
    }
    return 0;
}

// rekurznivne, protoze mame volitelne mnozstvi argumentu
int check_args(token_t *token){
    getNextToken(token);

    if (token->type == T_Keyword_String){
        getNextToken(token);

        if(token->type != T_Var_id)
            return 1;
        
        getNextToken(token);
        if(token->type == T_R_r_par){
            return 0;
        }
        else if(token -> type == T_Comma){
            return check_args(token);
        }
        else{
            return 1;
        }
    }
    else if (token->type == T_Keyword_Int){
        getNextToken(token);

        if(token->type != T_Var_id)
            return 1;
        
        getNextToken(token);
        if(token->type == T_R_r_par){
            return 0;
        }
        else if(token -> type == T_Comma){
            return check_args(token);
        }
        else{
            return 1;
        }
    }

    else if (token->type == T_Keyword_Float){
        getNextToken(token);

        if(token->type != T_Var_id)
            return 1;
        
        getNextToken(token);
        if(token->type == T_R_r_par){
            return 0;
        }
        else if(token -> type == T_Comma){
            return check_args(token);
        }
        else{
            return 1;
        }
    }
    else if(token -> type == T_R_r_par){
        return 0;
    }

    else{
        return 1;
    }
    
}

int check_call_args(token_t *token){
        
       getNextToken(token);
        if(token->type != T_Var_id)
            return 1;
        
        getNextToken(token);
        if(token->type == T_R_r_par){
            return 0;
        }
        else if(token -> type == T_Comma){
            return check_call_args(token);
        }
        else{
            return 1;
        }
}

//generujeme random stringy pro LABELs
char* make_random_string(){
    char const abeceda[]= "abcdefghijklmnopqrstuvwxyz0123456789";
    char* output = malloc(sizeof(char)*7);
    int random;
    output[0] = 'L';
    for(int i = 1; i<7; i++){
        random = rand()%35;
        output[i] = abeceda [random];
    }
    output[7] = '\0';
    return output;
}