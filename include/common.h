/**
 * @file common.h
 *
 * Functions and macros used in whole project
 *
 * IFJ project 2022
 *
 * @author <xstrel03> Matyáš Strelec
 *
 */

#ifndef COMMON_H
#define COMMON_H

/**
 * @brief Macro for error handling
 * 
 * Prints the error code with message and data, returns the error code
 * 
 * @param code Error code
 * @param token Token which caused the error
 * 
 * @return int Error code
 */

#define error_exit(code, token) \
    fprintf(stderr, "Error %d on line %d near %s\n", code, token->line, token->data);\
    return code;\

#endif
