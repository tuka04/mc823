#include "tempo.h"

void newMedia(Media **m, char *nome){
  *m=(Media *)malloc(sizeof(struct headTempo));
  (*m)->medida=(char *)malloc(sizeof(char)*strlen(nome));
  memset((*m)->medida,'\0',strlen(nome));
  memmove((*m)->medida,nome,strlen(nome));
  (*m)->media=00.00;
  (*m)->desvio=00.00;
  (*m)->length=0;
}
void calcMedia(Media **m){
  Tempo *t=(*m)->dados;
  double c=00.00;
  while(t!=NULL){
    c+=t->decorrido;
    t=t->prox;
  }
  (*m)->media=c/(double)(*m)->length;
  //desvio
  t=(*m)->dados;
  c=00.00;
  while(t!=NULL){
    c+=((t->decorrido-(*m)->media)*(t->decorrido-(*m)->media));
    t=t->prox;
  }
  (*m)->desvio=sqrt((1.00/((double)(*m)->length)*c));
}
void insertTempo(Media **m,double i,double f){
  Tempo *t=(Tempo *)malloc(sizeof(struct tempo));
  t->inicio=i;
  t->fim=f;
  t->decorrido=f-i;
  t->prox=NULL;
  Tempo *aux=(*m)->dados;
  if(aux==NULL){
    (*m)->dados = t;
  }
  else{
    while(aux->prox!=NULL)
      aux=aux->prox;
    aux->prox=t;
  }
  (*m)->length++;
}

void readFileMedia(Media **m, char *fname){
  FILE *f=openFile(fname,"r");
  char buffer[ARCHIVES_TAM_BUFFER];
  memset(buffer,'\0',ARCHIVES_TAM_BUFFER);
  double val;
  while(fgets(buffer, ARCHIVES_TAM_BUFFER, f)){
    val=atof(buffer);
    insertTempo(m,0,val);
  }
}

void writeFileMedia(Media **m){
  char *str = (char *)malloc(sizeof(char)*(strlen("./estat/")+strlen((*m)->medida)));
  memset(str,'\0',strlen(str));
  strcat(str,"./estat/");
  strcat(str,((*m)->medida));
  FILE *f=openFile(str,"w+");
  fprintf(f,"%lf (+-) %lf\n",(*m)->media, (*m)->desvio);
  fclose(f);
}

int main(int argc, char *argv[]){
  Media *m[NUM_OPCOES_MENU*2];
  char files[NUM_OPCOES_MENU*2][10];
  strcpy(files[0],"mtt_1\0");
  strcpy(files[1],"mtt_2\0");
  strcpy(files[2],"mtt_3\0");
  strcpy(files[3],"mtt_4\0");
  strcpy(files[4],"mtt_5\0");
  strcpy(files[5],"mtt_6\0");
  strcpy(files[6],"mtp_1\0");
  strcpy(files[7],"mtp_2\0");
  strcpy(files[8],"mtp_3\0");
  strcpy(files[9],"mtp_4\0");
  strcpy(files[10],"mtp_5\0");
  strcpy(files[11],"mtp_6\0");
  char rfiles[NUM_OPCOES_MENU*2][15];
  strcpy(rfiles[0],"./estat/tt_1\0");
  strcpy(rfiles[1],"./estat/tt_2\0");
  strcpy(rfiles[2],"./estat/tt_3\0");
  strcpy(rfiles[3],"./estat/tt_4\0");
  strcpy(rfiles[4],"./estat/tt_5\0");
  strcpy(rfiles[5],"./estat/tt_6\0");
  strcpy(rfiles[6],"./estat/tp_1\0");
  strcpy(rfiles[7],"./estat/tp_2\0");
  strcpy(rfiles[8],"./estat/tp_3\0");
  strcpy(rfiles[9],"./estat/tp_4\0");
  strcpy(rfiles[10],"./estat/tp_5\0");
  strcpy(rfiles[11],"./estat/tp_6\0");
  int i=0;
  for(i=0;i<NUM_OPCOES_MENU*2;i++){
    newMedia(&m[i],files[i]);
    readFileMedia(&m[i],rfiles[i]);
    calcMedia(&m[i]);
    writeFileMedia(&m[i]);
  }
  return 0;
}
