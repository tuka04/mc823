#include "log.h"

void writeLog(char *title, char *msg, char *extra){
  FILE *f;
  const time_t timer = time(NULL);
  f = fopen(LOG_FILE,"a+");
  if(f==NULL){
    printError(ERROR_LOG_FILE);
    exit(1);
  }
  fprintf(f,"%s (%s): %s %s\n",title,ctime(&timer),msg,extra);
  fclose(f);
}

void writeDoubleToFile(char *file,char *mode,double num){
  FILE *f=NULL;
  f=fopen(file,mode);
  if(f==NULL){
    printError(ERROR_LOG_FILE);
    exit(1);
  }
  fprintf(f,"%lf\n",num);
  fclose(f);
}
