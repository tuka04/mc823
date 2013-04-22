#include "archives.h"

FILE *openFile(char *nome, char *param){
  FILE *f;
  //abre em modo leitura
  f = fopen(nome, param);
  if(f == NULL){
      printError(ERROR_FILE_OPEN);
      exit(0);
  }
  return f;
}

void readFileBooks(FILE *f, AVL *avl){
  char del='#';
  char delAutores=';';
  char **arr=NULL;//array
  char **arrAutores=NULL;//array p/ split de autores
  char buffer[ARCHIVES_TAM_BUFFER];
  int col=0; //coluna que esta sendo lida
  int len=0;
  int i=0;
  Autores autores;
  while(fgets(buffer, ARCHIVES_TAM_BUFFER, f)){
    arr=split(buffer,del,&col);
    if(col!=NUM_COLUNAS_BOOKS){
      printError(ERROR_NUM_COLUNAS_BOOKS);
      exit(1);
    }
    else{
      Livro *l = (Livro *)malloc(sizeof(struct book));
      arrAutores=split(arr[AUTORES],delAutores,&col);//split autores
      newAutoresList(&autores);
      for(i=0;i<col;i++)
	insertAutoresList(&autores,arrAutores[i]);
      l->autores = autores;//(Autor *)malloc(sizeof(struct autor));
      //      memmove(l->autores,autores,sizeof(autores));
      len=strlen(arr[ISBN]);
      l->isbn=(char *)malloc(sizeof(char)*len+1);
      memmove(l->isbn,arr[ISBN],len+1);
      len=strlen(arr[TITULO]);
      l->titulo=(char *)malloc(sizeof(char)*len+1);
      memmove(l->titulo,arr[TITULO],len+1);
      len=strlen(arr[ANO]);
      l->ano=(char *)malloc(sizeof(char)*len+1);
      memmove(l->ano,arr[ANO],len+1);
      len=strlen(arr[DESCRICAO]);
      l->desc=(char *)malloc(sizeof(char)*len+1);
      memmove(l->desc,arr[DESCRICAO],len+1);
      len=strlen(arr[EDITORA]);
      l->editora=(char *)malloc(sizeof(char)*len+1);
      memmove(l->editora,arr[EDITORA],len+1);
      l->estoque=atoi(arr[ESTOQUE]);
      insereAVL(avl,l);
    }
  }
}


