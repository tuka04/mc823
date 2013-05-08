/**
 * Arvore AVL - (Adelson-Velskii e Landis)
 * Fator de Balanceamento altura_direita - altura_esquerda
 **/
#ifndef _H_AVL
#define _H_AVL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "books.h"


typedef struct avl {
  int bal;/*balanceamento avl*/
  char *id;/*id*/
  struct avl *dir;/*direita*/
  struct avl *esq;/*esquerdo*/
  void *dado;/*dado qualquer (estrutura)*/
}*AVL,AVL_NO;
/**
 * Inicializa uma arv avl;
 * @param AVL *avl;
 **/
void newAVLTree(AVL *avl);
/**
 * Retorna altura de uma avl
 * @param AVL avl
 * @return int 
 **/
int altura(AVL arv);
/**
 * Verifica que se uma arvore eh avl
 **/
int verifica_AVL(AVL arv);
void LL(AVL *avl);
void RR(AVL *avl);
void LR(AVL *avl);
void RL_X(AVL *avl);
int aux_insereAVL(AVL *avl, Livro *l, int *cresceu);
int insereAVL(AVL *avl, Livro *l);
/**
 * @function : Grava todos os ids separados pelo delimitador em uma string
 * @param AVL avl : estrutura avl
 * @param char **str : string q contera os ids
 * @param char *del : delimitador entre ids
 * @example : str tera o formato delstrdelstr....
 **/
void avlIdToStr(AVL avl, char **str, char *del);
/**
 * Converte todo conteudo da estrutura em uma string
 * @param AVL avl : estrutura avl
 * @param char **str : string q contera os ids
 * @param char *del : delimitador entre ids
 * @example : str tera o formato delstrdelstr....
 **/
void avlToStr(AVL avl, char **str, char *del);
/**
 * calcula o total de caracter presente em toda estrutura avl
 * @param AVL avl : estrutura
 **/
int totAVLchar(AVL avl);
/**
 * Retorna o total de caracteres que tem em todos isbn presentes
 * na estrutura.
 * @param AVL avl
 * @return int total de caracters
 **/
int totISBNchar(AVL avl);
/**
 * Retorna o total de elementos na arv avl
 * na estrutura.
 * @param AVL avl
 * @return int total de elem.
 **/
int totElemAVL(AVL avl);

/**
 * Limpa memoria ocupada pela arv
 * @param AVL avl
 **/
void freeAVL(AVL_NO *avl);

/**
 * Retorna o elemento encontrado ou NULL
 * @param AVL *avl
 * @param char *id
 **/
AVL_NO *getAVLElemById(AVL_NO *avl, char *id);

#endif

