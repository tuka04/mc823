/**
 * Manipula arquivos.
 * @version: 1.1
 * @changelog
 * - funcao isThisExtension(a,b);
 * - char *getFileExtension(char *nomeArq)
 * + include avl.h
 * + include error.h
 * + void readFileBooks(FILE *f, AVL *avl);
 **/

#ifndef _H_ARCHIVES
#define _H_ARCHIVES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "error.h"
#include "common.h"
#include "avl.h"

#define ARCHIVES_TAM_BUFFER 65536
#define NUM_COLUNAS_BOOKS 7
#define COCOZAO 2

/**
 * Funcao que abre o arquivo 
 * @param: char *nome
 * @param: char *param - parametro para abertura do arquivo
 * @param: FilaErr *pErro - ponteiro para a fila de descricao de erros
 **/
FILE *openFile(char *nome, char *param);

/**
 * @function : Funcao que le um arquivo separado por um delimitador, que contem
 * uma lista de livro (ver sequencia de dados em  books.h BOOKS_FILE_ORDER)
 * e salva na estrutura de arvore avl
 * @param FILE *f : file resource
 * @param AVL *avl : estrutura
 * @see books.h , avl.h , README
 **/
void readFileBooks(FILE *f, AVL *avl);

#endif
