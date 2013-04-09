#ifndef _H_BOOKS
#define _H_BOOKS
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "common.h"

#define FILE_BOOK "./common/livros/livros"
#define BOOK_NOT_FOUND "ISBN nao encontrado!\n"
typedef struct autor {
  char *nome; //nome do campo
  struct autor *prox;//apontador para o proximo
} *Autores, Autor;

typedef struct book {
  char *isbn;//id
  char *titulo;//title
  Autores autores;//estrutura de autor
  char *desc;//descricao
  char *editora;//editora
  char *ano;//data (apenas ano)
  int estoque;//numero de exemplares
} Livro;

enum BOOKS_FILE_ORDER {
  ISBN = 0,
  AUTORES,
  TITULO,
  ANO,
  DESCRICAO,
  EDITORA,
  ESTOQUE
};
/**
 * Cria nova lista para autores
 * @param Autores *a
 **/
void newAutoresList(Autores *a);
/**
 * Insere um novo autor na lista
 * @param Autores *a
 **/
void insertAutoresList(Autores *a, char *nome);
/**
 * Limpa a lista de autores
 * @param Autores *a
 **/
void freeAutoresList(Autores *a);
/**
 * Retorna o numero de autores de um livro
 * @param Autor *a : estrutura Autor
 * @return int
 **/
int getAutoresNum(Autor *a);
/**
 * Retorna o numero bytes dos autores de um livro
 * @param Autor *a : estrutura Autor
 * @return int
 **/
int getAutoresNumBytes(Autor *a);
/**
 * Retorna o numero bytes de um livro
 * @param Livro *l : estrutura Livro
 * @return int
 **/
int getBookNumBytes(Livro *l);
/**
 * Converte uma lista de autores em string com um delimitador
 * @param Autor *n : lista de autores
 * @param char *del : delimitador
 * @return char *str
 **/
char *autoresNodeToStr(Autor *n,char *del);
/**
 * Converte um livro em string
 * @param Autor *n : lista de autores
 * @return char *str
 **/
char *bookNodeToStr(Livro *l);
int getBookNodeSize();
#endif
