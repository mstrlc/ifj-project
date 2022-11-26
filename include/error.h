/**
 * @file error.h
 *
 * Definitions of error codes from assignment
 *
 * IFJ project 2022
 *
 * @author <xstrel03> Matyáš Strelec
 *
 */

#ifndef ERROR_H
#define ERROR_H

// Chyba v programu v ramci lexikalni analyzy
// (chybna struktura aktualniho lexemu)
#define ERR_LEXEME 1

// Chyba v programu v ramci syntakticke analyzy
// (chybna syntaxe programu, chybejici hlavicka, atp.)
#define ERR_SYNTAX 2

// Semanticka chyba v programu -
// nedefinovana funkce, pokus o redefinice funkce
#define ERR_UNDEF_REDEF_FUN 3

// Semanticka/behova chyba v programu -
// spatny pocet/typ parametru u volani funkce ci typ navratove hodnoty z funkce
#define ERR_WRONG_PARAM_RET 4

// Semanticka chyba v programu -
// pouziti nedefinovane promenne
#define ERR_UNDEF_VAR 5

// Semanticka/behova chyba v programu -
// chybejici/prebyvajici vyraz v prikazu navratu z funkce
#define ERR_MISS_EXCESS_RET 6

// Semanticka/behova chyba typove kompatibility v aritmetickych,
// retezcovych a relacnich vyrazech
#define ERR_EXPR_TYPE 7

// Ostatni semanticke chyby
#define ERR_OTHER_SEM 8

// Interni chyba prekladace tj. neovlivnena vstupnim programem
// (napr. chyba alokace pameti atd.)
#define ERR_INTERNAL 99

#endif
