#include "avl.h"

void newAVLTree(AVL *avl){
  *avl=NULL;
}

int altura(AVL avl){
  if (avl == NULL)
    return 0;
  int hesq = altura(avl->esq);  
  int hdir = altura(avl->dir);
  return hesq > hdir ? hesq + 1 : hdir + 1;
}

int verifica_AVL(AVL avl){
  if (avl==NULL)
    return 1;
  return abs(altura(avl->dir) - altura(avl->esq)) <= 1;
}

void LL(AVL *avl){
  AVL_NO *b = *avl;
  AVL_NO *a = b->esq;
  b->esq = a->dir;
  a->dir = b;
  a->bal = 0;
  b->bal = 0;
  *avl = a;
}

void RR(AVL *avl){
  AVL_NO *a = *avl;
  AVL_NO *b = a->dir;
  a->dir = b->esq;
  b->esq = a;
  a->bal = 0;
  b->bal = 0;
  *avl = b;  
}  
void LR(AVL *avl){
  AVL_NO *c = *avl;
  AVL_NO *a = c->esq;
  AVL_NO *b = a->dir;
  c->esq = b->dir;
  a->dir = b->esq;
  b->esq = a;
  b->dir = c;
  switch(b->bal) {
  case -1:
    a->bal = 0;
    c->bal = 1;
    break;
  case 0:
    a->bal = 0;
    c->bal = 0;
    break;
  case +1:
    a->bal = -1;
    c->bal = 0;
    break;
  }
  b->bal = 0;
  *avl = b; 
}

void RL(AVL *avl){
  AVL_NO *a = *avl;
  AVL_NO *c = a->dir;
  AVL_NO *b = c->esq;
  c->esq = b->dir;
  a->dir = b->esq;
  b->esq = a;
  b->dir = c;
  switch(b->bal) {
  case -1:
    a->bal = 0;
    c->bal = 1;
    break;
  case 0:
    a->bal = 0;
    c->bal = 0;
    break;
  case +1:
    a->bal = -1;
    c->bal = 0;
    break;
  }
  b->bal = 0;
  *avl = b; 
}

int aux_insereAVL(AVL *avl, Livro *l, int *cresceu){
  if (*avl == NULL) {
    AVL_NO *no = (AVL_NO *)malloc(sizeof(struct avl));
    //    Livro *temp = (Livro *)malloc(sizeof(struct book));
    no->id = (char *)malloc(sizeof(char)*strlen(l->isbn)+1);
    memset(no->id,'\0',strlen(l->isbn)+1);
    memmove(no->id,l->isbn,strlen(l->isbn)+1);
    //    memmove(temp,l,sizeof(l));
    no->bal = 0;
    no->esq = NULL;
    no->dir = NULL;
    no->dado=(void *)l;
    *avl = no;
    *cresceu = 1;
    return 1;
  }
  if(memcmp(l->isbn,((Livro *)(*avl)->dado)->isbn,strlen(l->isbn))>0){
    if(aux_insereAVL(&(*avl)->esq,l,cresceu)) {
      if(*cresceu) {
	switch((*avl)->bal) {
	case -1:
	  if((*avl)->esq->bal == -1)
	    LL(avl);
	  else
	    LR(avl);
	  *cresceu = 0;
	  break;
	case 0:
	  (*avl)->bal = -1;
	  *cresceu = 1;
	  break;
	case +1:
	  (*avl)->bal = 0;
	  *cresceu = 0;
	  break;
	}
      }
      return 1;
    }
    else
      return 0;
  }
  if(aux_insereAVL(&(*avl)->dir,l,cresceu)) {
    if(*cresceu) {
      switch((*avl)->bal) {
      case -1:
	(*avl)->bal = 0;
	*cresceu = 0;
	break;
      case 0:
	(*avl)->bal = +1;
	*cresceu = 1;
	break;
      case +1:
	if ((*avl)->dir->bal == +1)
	  RR(avl);
	else
	  RL(avl);
	*cresceu = 0;
	break;
      }
    }
    return 1;
  }
  else
    return 0;
}

int insereAVL(AVL *avl, Livro *l) {
  int cresceu;
  return aux_insereAVL(avl,l,&cresceu);
}

/**
 * @function : Grava todos os ids separados pelo delimitador em uma string
 * @param AVL avl : estrutura avl
 * @param char **str : string q contera os ids
 * @param char *del : delimitador entre ids
 * @example : str tera o formato delstrdelstr....
 **/
void avlIdToStr(AVL avl, char **str, char *del){
  if(avl!=NULL){
    avlIdToStr(avl->esq,str,del);
    strcat(*str,del);
    strcat(*str,avl->id);
    avlIdToStr(avl->dir,str,del);
  }
}

void avlToStr(AVL avl, char **str, char *del){
  if(avl!=NULL){
    avlToStr(avl->esq,str,del);
    strcat(*str,bookNodeToStr((Livro *)avl->dado));
    strcat(*str,"\n");
    avlToStr(avl->dir,str,del);
  }
}

int totAVLchar(AVL avl){
  if(avl==NULL)
    return 0;
  int e=totAVLchar(avl->esq);
  int d=totAVLchar(avl->dir);
  return e+d+getBookNumBytes((Livro *)avl->dado);
}

int totISBNchar(AVL avl){
  if(avl==NULL)
    return 0;
  int e = totISBNchar(avl->esq);
  int d = totISBNchar(avl->dir);
  return (e+d+strlen(avl->id));
}

int totElemAVL(AVL avl){
  if(avl==NULL)
    return 0;
  int e = totElemAVL(avl->esq);
  int d = totElemAVL(avl->dir);
  return (e+d+1);
}

void freeAVL(AVL_NO *avl){
  if(avl!=NULL){
    freeAVL(avl->esq);
    freeAVL(avl->dir);
    free(avl);
  }
}

AVL_NO *getAVLElemById(AVL_NO *avl, char *id){
  if(&(*avl) != NULL){
    AVL_NO *aux = avl;
    if(strcmp(aux->id,id)==0)
      return aux;
    else if(strcmp(aux->id,id)<0)
      return getAVLElemById(aux->esq,id);
    return getAVLElemById(aux->dir,id);
  }
  return NULL;
}

