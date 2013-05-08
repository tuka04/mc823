#include "error.h"

void printError(int e){
  switch(e){
  case ERROR_SOCKET:
    perror("socket:: naco foi possivel criar um novo socket\n");
    break;
  case ERROR_SOCKET_CLIENT:
    perror("socket:: nao foi possivel conectar ao cliente\n");
    break;
  case ERROR_SOCKET_RCV_MSG:
    perror("socket:: erro ao receber mensagem\n");
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
  case ERROR_USAGE_TEST:
    printf("erro de entrada: tente ./test <endereco_ip> <porta> para iniciar o cliente (o servidor deve estar up!)\n");
    break;
  case ERROR_FILE_OPEN:
    perror("archives:: Erro ao abrir arquivo\n");
    break;
  case ERROR_NUM_COLUNAS_BOOKS:
    printf("archives:: Arquivo com conteudo de livros corrompido. VEJA README PARA MAIS DETALHES\n");
  case ERROR_UDP_MAX_RETRY:
    perror("udp_sendto:: Limite maximo de tentativas de envio, altere UDP_MAX_RETRY \n");
    break;
  }
}

