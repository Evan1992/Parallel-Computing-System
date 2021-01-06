/*
 * #include guards
 *  #ifndef PUBLIC_H
 *  #define PUBLIC_H
 *  ...
 *  #endif
 *  It prevents double declaration of any identifiers such as struct, static variables 
 */

#ifndef PUBLIC_H
#define PUBLIC_H

#define N 512    /* Matrix size */
typedef struct {float r; float i;} complex;

#endif