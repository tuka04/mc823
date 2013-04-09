#include "books.h"

void newAutoresList(Autores *a){
  *a=NULL;
}

void insertAutoresList(Autores *a, char *nome){
  Autor *autor=(Autor *)malloc(sizeof(struct autor));
  autor->nome=(char *)malloc(sizeof(char)+strlen(nome));
  autor->prox=NULL;
  memmove(autor->nome,nome,strlen(nome)+1);
  if(*a==NULL)
    *a = autor;
  else{
    Autor *aux = *a;
    while(aux->prox!=NULL)
      aux=aux->prox;
    aux->prox=autor;
  }
}

void freeAutoresList(Autores *a){
  Autor *aux=*a;
  while(aux!=NULL){
    Autor *r=aux;
    aux=aux->prox;
    free(r);
  }
}
int getAutoresNum(Autor *a){
  if(a==NULL)
    return 0;
  return 1+getAutoresNum(a->prox);
}
int getAutoresNumBytes(Autor *a){
  Autor *aux = a;
  int len=0;
  while(aux!=NULL){
    len+=strlen(aux->nome)+1;
    aux=aux->prox;
  }
  return len;
}
int getBookNumBytes(Livro *l){
  int len=0;
  len=strlen(l->isbn)+strlen(l->titulo)+strlen(l->desc)+strlen(l->editora);
  len+=strlen(l->ano)+sizeof(int)+6;
  len+=getAutoresNumBytes(l->autores);
  return len;
}
char *autoresNodeToStr(Autor *n,char *del){
  Autor *a=n;
  int len=getAutoresNumBytes(n)+getAutoresNum(n)*strlen(del)+1;
  char *str=(char *)malloc(len);
  memset(str,'\0',len);
  while(a!=NULL){
    strcat(str,a->nome);
    strcat(str,del);
    a=a->prox;
  }
  return str;
}
int getBookNodeSize(){
  char *isbn="\nISBN: ";
  char *tit="\nTITULO: ";
  char *desc="\nDESCRICAO: ";
  char *autores="\nAutor(es): ";
  char *edit="\nEditora: ";
  char *ano="\nAno: ";
  char *estoque="\nQuantidade: ";
  return strlen(isbn)+strlen(tit)+strlen(desc)+strlen(autores)+strlen(edit)+strlen(ano)+strlen(estoque)+10;
}
char *bookNodeToStr(Livro *l){
  int len = getBookNumBytes(l);
  char *isbn="\nISBN: ";
  char *tit="\nTITULO: ";
  char *desc="\nDESCRICAO: ";
  char *autores="\nAutor(es): ";
  char *edit="\nEditora: ";
  char *ano="\nAno: ";
  char *estoque="\nQuantidade: ";
  len+=strlen(isbn)+strlen(tit)+strlen(desc)+strlen(autores)+strlen(edit)+strlen(ano)+strlen(estoque);
  len+=4;
  //agora ja temos o tamanho total da msg len+4 (\n\0)
  char *msg=(char *)malloc(len);//\n
  memset(msg,'\0',len);
  strcat(msg,isbn);
  strcat(msg,l->isbn);
  strcat(msg,tit);
  strcat(msg,l->titulo);
  strcat(msg,desc);
  strcat(msg,l->desc);
  strcat(msg,autores);
  strcat(msg,autoresNodeToStr(l->autores,", "));
  strcat(msg,edit);
  strcat(msg,l->editora);
  strcat(msg,ano);
  strcat(msg,l->ano);
  strcat(msg,estoque);
  strcat(msg,my_itoa(l->estoque,10));
  strcat(msg,"\n");
  return msg;
}
