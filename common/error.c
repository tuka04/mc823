#include "error.h"

void printError(int e){
  switch(e){
  case ERROR_SOCKET:
    perror("socket:: naco foi possivel criar um novo socket\n");
    break;
  case ERROR_SOCKET_CLIENT:
    perror("socket:: nao foi possivel conectar ao cliente\n");
    break;
  case ERROR_SOCKET_SEND:
    perror("socket:: nao foi possivel enviar mensagem para o servidor\n");
    break;
  case ERROR_SOCKET_SERVER_ERROR:
    perror("send_msg:: ocorreu um erro na comunicacao com o servidor, tente novamente");
    break;
  case ERROR_SOCKET_CLIENT_CLOSED:
    perror("receive_msg:: cliente fechou a conexao");
    break;
  case ERROR_SOCKET_SERVER_CLOSED:
    perror("send_msg:: servidor fechou a conexao");
    break;
  case ERROR_SOCKET_SERVER_BIND:
    perror("start_server:: ocorreu um erro com o bind");
    break;
  case ERROR_SOCKET_SERVER_LISTEN:
    perror("start_server:: ocorreu um erro com o listen");
    break;
  case ERROR_SOCKET_SERVER_ACCEPT:
    perror("start_server:: ocorreu um erro com o accept");
    break;
  case ERROR_USAGE_MAIN:
    printf("erro de entrada: tente ./main <num_porta> (para iniciar servidor) ou ./main <endereco_ip> <porta> para iniciar o cliente\n");
    break;
  case ERROR_FILE_OPEN:
    perror("archives:: Erro ao abrir arquivo\n");
    break;
  case ERROR_NUM_COLUNAS_BOOKS:
    printf("archives:: Arquivo com conteudo de livros corrompido. VEJA README PARA MAIS DETALHES\n");
    break;
  }
}
/*char* my_itoa(int val, int base){	
  static char buf[32] = {0};
  int i = 30;
  for(; val && i ; --i, val /= base)
    buf[i] = "0123456789abcdef"[val % base];
  return &buf[i+1];
  }*/
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
