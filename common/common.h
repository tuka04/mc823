#ifndef _H_COMMON
#define _H_COMMON
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CHAR_NEW_LINE "\n"
/**
 * Convert inteiro na base escolhida para string
 * @param int val
 * @param int base
 * @see http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
 **/
char* my_itoa(int val, int base);

/**
 *@function :   copys s[a] -> s[b] into t.
 *@param char *t : target string (should be malloc(a + b + 1))
 *@param char *s : source string
 *@param int a : startpoint in source string (must be > 0 )
 *@param int b : endpoint in source string (must be < strlen(s))
 **/  
void poscpy(char *t,char *s,int a,int b);
/**
 * Retorna um vetor de string que foi dividida em tokens.
 * @param char *str : string original
 * @param char n : delimitador
 * @param int *lenght : escreve a quantidade de tokens geradas (count do retorno)
 * @return char ** array
 * @see http://www.vivaolinux.com.br/topico/C-C++/Split-em-C
 **/
char** split(char* str,char n,int *length);

#endif
