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
  memset(msg,'\0',TCP_BUF_SIZE);
  char recvMsg[tcpBuffSize];//received msg
  bool transmit=true;//var de saida
  printf("Para sair tecle %s\n",TCP_COMMAND_CLOSE_CONNECTION);
  int rc=0;
  do{
    if(strcmp(msg,"4")==0){
      do{
	max = recv(sDesc,(void *)recvMsg,tcpBuffSize,0);//-1 error, 0 closed
	if(max<0){
	  printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	  close(sDesc);
	  exit(1);
	}
	recvMsg[max]='\0';
	printf("%s",recvMsg);
	if(rc<tcpBuffSize)
	  send(sDesc,TCP_MSG_ACK,strlen(TCP_MSG_ACK),0);//send message     
	rc+=max;
      }while(rc<tcpBuffSize);
    }
    else{
      if(strlen(msg)==0)
	send(sDesc,TCP_COMMAND_MENU,strlen(TCP_COMMAND_MENU),0);
      max = recv(sDesc,(void *)recvMsg,tcpBuffSize,0);//-1 error, 0 closed
      recvMsg[max]='\0';
      printf("%s",recvMsg);
    }
    rc=0;
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

void build_teste(int sDesc,struct in_addr addr, int port){
  char msgMaxTCPBuffSize[10];
  memset(msgMaxTCPBuffSize,'\0',10);
  int max = recv(sDesc,msgMaxTCPBuffSize,10,0);//-1 error, 0 closed
  if(max<0){
    printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
    close(sDesc);
    exit(1);
  }
  msgMaxTCPBuffSize[max]='\0';
  send(sDesc,TCP_COMMAND_MENU,strlen(TCP_COMMAND_MENU),0);//send message
  int tcpBuffSize=atoi(msgMaxTCPBuffSize);
  char recvMsg[tcpBuffSize];//received msg
  struct timeval tvini, tvend;
  double ini=00.00,end=00.00;
  int *msgT=getTesteMsg();
  char testes[7][18];
  strcpy(testes[0],"./estat/tt_menu\0");
  strcpy(testes[1],"./estat/tt_1\0");
  strcpy(testes[2],"./estat/tt_2\0");
  strcpy(testes[3],"./estat/tt_3\0");
  strcpy(testes[4],"./estat/tt_4\0");
  strcpy(testes[5],"./estat/tt_5\0");
  strcpy(testes[6],"./estat/tt_6\0");
  char isbn[14][13];
  strcpy(isbn[0],"978853526-0\0");
  strcpy(isbn[1],"978052007-4\0");
  strcpy(isbn[2],"857302773-8\0");
  strcpy(isbn[3],"850109104-9\0");
  strcpy(isbn[4],"843760494-X\0");
  strcpy(isbn[5],"207036002-4\0");
  strcpy(isbn[6],"081297215-5\0");
  strcpy(isbn[7],"978858041-1\0");//errado
  //  strcpy(isbn[7],"850197213-4");//errado
  strcpy(isbn[8],"978858041-1\0");
  strcpy(isbn[9],"978857679-9\0");
  strcpy(isbn[10],"978857608-5\0");
  strcpy(isbn[11],"978853990-1\0");
  strcpy(isbn[12],"978857521-2\0");
  strcpy(isbn[13],"978850220-0\0");
  int i=0,random=0;
  int rc=0;
  max = recv(sDesc,recvMsg,tcpBuffSize,0);//menu
  recvMsg[max]='\0';
  printf("%s",recvMsg);
  for(i=0;i<NUM_TESTES*NUM_OPCOES_MENU;i++){
    rc=0;
    gettimeofday(&tvini, NULL);//marca envio
    printf("%d",msgT[i]);
    max = send(sDesc,my_itoa(msgT[i],10),strlen(my_itoa(msgT[i],10)),0);
    if(msgT[i]==2 || msgT[i]==3 || msgT[i]==5 || msgT[i]==6){//precisa de isbn
      do{
	max = recv(sDesc,recvMsg,tcpBuffSize,0);
	recvMsg[max]='\0';
	printf("%s",recvMsg);
      }while(strcmp(recvMsg,TCP_MSG_ISBN_REQUIRED)!=0 || max>strlen(TCP_MSG_ISBN_REQUIRED));
      if(strcmp(recvMsg,TCP_MSG_ISBN_REQUIRED)==0){
	random=rand()%13;
	max = send(sDesc,isbn[random],strlen(isbn[random]),0);
	printf(" SEND %s+ ",isbn[random]);
	if(msgT[i]==5){//precisa de senha && qntidade
	  do{
	    max = recv(sDesc,recvMsg,tcpBuffSize,0);
	    recvMsg[max]='\0';
	    printf("%s",recvMsg);
	  }while(strcmp(recvMsg,TCP_MSG_LOGIN_REQUIRED)!=0 || max>strlen(TCP_MSG_LOGIN_REQUIRED));
	  if(strcmp(recvMsg,TCP_MSG_LOGIN_REQUIRED)==0){
	    max = send(sDesc,SENHA,strlen(SENHA),0);
	    printf(" SEND %s+ ",SENHA);
	    max = recv(sDesc,recvMsg,tcpBuffSize,0);
	    recvMsg[max]='\0';
	    printf("%s",recvMsg);
	    if(strcmp(recvMsg,TCP_MSG_QTD_REQUIRED)==0){//qntidade
	      random=rand()%1000;
	      max = send(sDesc,my_itoa(random,10),strlen(my_itoa(random,10)),0); 
	      printf(" SEND %s+ ",my_itoa(random,10));
	      max = recv(sDesc,recvMsg,tcpBuffSize,0);//opcao
	      recvMsg[max]='\0';
	    }//qntd
	  }
	}
	else{
	  max = recv(sDesc,recvMsg,tcpBuffSize,0);//opcao
	  recvMsg[max]='\0';
	}
      }//isbn
      printf("@%s@",recvMsg);
    }
    else if(msgT[i]==4){
      rc=0;
      do{
	max = recv(sDesc,(void *)recvMsg,tcpBuffSize,0);//-1 error, 0 closed
	if(max<0){
	  printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	    close(sDesc);
	    exit(1);
	}
	recvMsg[max]='\0';
	printf("@%s@",recvMsg);
	if(rc==0)
	  send(sDesc,TCP_MSG_ACK,strlen(TCP_MSG_ACK),0);//send message     
	rc+=max;
      }while(rc<tcpBuffSize);
    }
    else{
      max = recv(sDesc,(void *)recvMsg,tcpBuffSize,0);//-1 error, 0 closed
      recvMsg[max]='\0';
      printf("@%s@",recvMsg);
    }
    gettimeofday(&tvend, NULL);//marca recebimento
    ini=DOUBLE_MILHAO*(double)(tvini.tv_sec)+(double)(tvini.tv_usec);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile(testes[msgT[i]],"a+",end-ini);
  }
}
int *getTesteMsg(){
  int *msg=(int *)malloc(sizeof(int)*NUM_OPCOES_MENU*NUM_TESTES);
  int i=0,j=0,c=0;
  for(i=0;i<NUM_TESTES;i++){
    for(j=0;j<NUM_OPCOES_MENU;j++){
      *(msg+c)=j+1;
      c++;
    }
  }
  return msg;
}
