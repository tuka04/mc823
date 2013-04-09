#include "tcp_client.h"

int conn(struct in_addr addr, int port){
  int sDesc;//socket descriptor
  int conn;//conexao
  struct sockaddr_in *s = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));//servidor
  socklen_t len = sizeof(struct sockaddr);//evitar: error: conversion to non-scalar type requested
  sDesc = socket(AF_INET,SOCK_STREAM,0);//IPv4 e TCP (sequencial,confiavel,2-way,byte-stream)
  if(sDesc<0){
    printError(ERROR_SOCKET);
    exit(1);
  }
  memset(s,0,sizeof(s));//write zero-valued bytes
  s->sin_family = AF_INET;
  s->sin_port = htons(port);//eh short nao long...nao funciova sem htons :(
  s->sin_addr = addr;
  //conexao
  conn = connect(sDesc,(struct sockaddr *)s,len);
  if(conn<0){
    printError(ERROR_SOCKET_CLIENT);
    close(sDesc);//lembrar de fechar o socket aberto
    exit(1);
  }
  free(s);
  return sDesc;
}

void send_msg(int sDesc,struct in_addr addr, int port){
  char msgMaxTCPBuffSize[10];
  memset(msgMaxTCPBuffSize,'\0',10);
  int max = recv(sDesc,msgMaxTCPBuffSize,10,0);//-1 error, 0 closed
  if(max<0){
    printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
    close(sDesc);
    exit(1);
  }
  msgMaxTCPBuffSize[max]='\0';
  send(sDesc,TCP_MSG_ACK,strlen(TCP_MSG_ACK),0);//send message
  int tcpBuffSize=atoi(msgMaxTCPBuffSize);
  char msg[TCP_BUF_SIZE];//msg a ser transmitida
  char recvMsg[tcpBuffSize];//received msg
  bool transmit=true;//var de saida
  printf("Para sair tecle %s\n",TCP_COMMAND_CLOSE_CONNECTION);
  do{
    max = recv(sDesc,recvMsg,tcpBuffSize,0);//-1 error, 0 closed
    if(max<0){
      printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
      close(sDesc);
      exit(1);
    }
    recvMsg[max]='\0';
    printf("%s",recvMsg);
    if(strcmp(TCP_COMMAND_CLOSE_CONNECTION,msg)==0){//saida?
      transmit=false;//nao transmitir mais
    }
    else{
      do{
	fgets(msg,TCP_BUF_SIZE-1,stdin);//lendo input
	msg[strlen(msg)-strlen(CHAR_NEW_LINE)]='\0';//remove new line
	if(strlen(msg)==0)
	  printf("Digite um comando valido!\n");
      }while(strlen(msg)==0);
      //temos que transmitir a msg de fechar conexao
      max = send(sDesc,msg,strlen(msg),0);//send message
      if(max<0){
	close(sDesc);//close socket
	if(max==-1){
	  exit(1);
	  sDesc = conn(addr,port);
	}
	else{
	  printError(ERROR_SOCKET_SERVER_ERROR);
	  exit(1);
	}
      }
    }
  }while(transmit);
}
