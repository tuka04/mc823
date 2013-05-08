#include "udp_client.h"

int openSocket(struct addrinfo *a){
  int sDesc;/*socket descriptor*/
  a->ai_family=AF_INET;/*IPv4*/
  a->ai_socktype=SOCK_DGRAM;/*upd entao, datagram*/
  a->ai_protocol=IPPROTO_UDP;
  sDesc = socket(a->ai_family,a->ai_socktype,a->ai_protocol);/*IPv4 e upd = datagram*/
  if(sDesc<0){/*erro?*/
    printError(ERROR_SOCKET);/*print erro*/
    exit(1);/*saida inesperada*/
  }
  /*nao eh orientado a conexao, como o tcp*/
  return sDesc;/*retorna sockdescriptor*/
}

void first_msg(int s, AI *a){
  int max=0;
  int r=0;
  do{
    max = sendto(s,TCP_MSG_HELLO,strlen(TCP_MSG_HELLO),0,a->ai_addr,a->ai_addrlen);
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
    if(LOG)
      writeLog("UDP_CLIENT::send",my_itoa(max,10),inet_ntoa(((SA_IN*)a->ai_addr)->sin_addr));
    r++;
    if(r>UDP_MAX_RETRY){
      printError(ERROR_UDP_MAX_RETRY);/*maximo atingido*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<strlen(TCP_MSG_HELLO));/*a msg nao foi devidamente entregue?*/
}

void send_ack(int s, AI *a){  
  int max=0;
  int r=0;
  do{
    max = sendto(s,TCP_MSG_ACK,strlen(TCP_MSG_ACK),0,a->ai_addr,a->ai_addrlen);
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
    r++;
    if(r>UDP_MAX_RETRY){
      printError(ERROR_UDP_MAX_RETRY);/*maximo atingido*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<strlen(TCP_MSG_ACK));
}

void send_msg(int sDesc,AI *a){
  char msgMaxTCPBuffSize[10];/*msg a ser enviada*/
  memset(msgMaxTCPBuffSize,'\0',10);/*zerando mem*/
  first_msg(sDesc,a);/*hello*/
  int max=0;
  /*recebendo msg*/
  max = recvfrom(sDesc,msgMaxTCPBuffSize,10,0,a->ai_addr,&(a->ai_addrlen));
  if(max<0){/*erro?*/
    printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
    close(sDesc);/*fechar o socket aberto*/
    exit(1);/*saida com erro*/
  }
  if(LOG) writeLog("UDP_CLIENT::rcv",msgMaxTCPBuffSize,"");
  send_ack(sDesc,a);/*ack*/
  /*-1 error, 0 closed*/
  msgMaxTCPBuffSize[max]='\0';/*limit msg*/
  /*msg recebida tera no maximo esse tamanho*/
  int tcpBuffSize=atoi(msgMaxTCPBuffSize);
  char msg[TCP_BUF_SIZE];/*msg a ser transmitida*/
  memset(msg,'\0',TCP_BUF_SIZE);/*zerando mem*/
  char recvMsg[tcpBuffSize];/*msg a ser recebida*/
  bool transmit=true;/*var de saida*/
  printf("Para sair tecle %s\n",TCP_COMMAND_CLOSE_CONNECTION);/*requisicao*/
  int rc=0;/*para verificar se a msg inteira foi recebida*/
  do{
    if(strcmp(msg,"4")==0){/*cond. se a msg a ser recebida eh mto grande*/
      do{
	/*recebendo msg*/
	max = recvfrom(sDesc,(void *)recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
	if(max<0){/*erro?*/
	  /*qual tipo?*/
	  printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	  close(sDesc);/*fechar socket*/
	  exit(1);/*saida inesperada*/
	}
	recvMsg[max]='\0';/*limit msg*/
	printf("%s",recvMsg);/*imprimi msg ja lida do buffer*/
	if(rc<tcpBuffSize)/*leu a msg por completo?*/
	  sendto(sDesc,TCP_MSG_ACK,strlen(TCP_MSG_ACK),0,a->ai_addr,a->ai_addrlen);/*quero mais msg*/
	rc+=max;/*bom*/
      }while(rc<tcpBuffSize);/*ja atingimos o tamanho?*/
    }
    else{/*msg curta*/
      if(strlen(msg)==0)/*quero menu?*/
	sendto(sDesc,TCP_COMMAND_MENU,strlen(TCP_COMMAND_MENU),0,a->ai_addr,a->ai_addrlen);
      /*recebendo menu*/
      max = recvfrom(sDesc,(void *)recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
      recvMsg[max]='\0';/*limit msg*/
      printf("%s",recvMsg);/*print msg recebida*/
    }
    rc=0;/*zerando auxiliar de msgs grandes*/
    if(strcmp(TCP_COMMAND_CLOSE_CONNECTION,msg)==0){/*saida?*/
      transmit=false;/*nao transmitir mais*/
    }
    else{/*msg curta*/
      do{
	fgets(msg,TCP_BUF_SIZE-1,stdin);/*lendo input*/
	msg[strlen(msg)-strlen(CHAR_NEW_LINE)]='\0';/*remove new line*/
	if(strlen(msg)==0)/*comando valido? ou seja, nao eh um simples \n*/
	  printf("Digite um comando valido!\n");
      }while(strlen(msg)==0);/*ateh um comando valido*/
      /*temos que transmitir a msg de fechar conexao*/
      max = sendto(sDesc,msg,strlen(msg),0,a->ai_addr,a->ai_addrlen);
      if(max<0){/*msg ok?*/
	close(sDesc);/*close socket*/
	printError(ERROR_SOCKET_SERVER_ERROR);/*print error*/
	exit(1);/*saida inesperada*/
      }
    }
  }while(transmit);/*ateh fim da transmissao*/
}

void build_teste(int sDesc,AI *a){
  char msgMaxTCPBuffSize[10];
  memset(msgMaxTCPBuffSize,'\0',10);
  first_msg(sDesc,a);/*hello*/
  int max = recvfrom(sDesc,msgMaxTCPBuffSize,10,0,a->ai_addr,&(a->ai_addrlen));
  if(max<0){
    printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
    close(sDesc);
    exit(1);
  }
  msgMaxTCPBuffSize[max]='\0';
  send_ack(sDesc,a);
  do{
    max=sendto(sDesc,TCP_COMMAND_MENU,strlen(TCP_COMMAND_MENU),0,a->ai_addr,a->ai_addrlen);
    if(max<0){
      printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
      close(sDesc);
      exit(1);
    }
  }while(max<1);
  int tcpBuffSize=atoi(msgMaxTCPBuffSize);
  char recvMsg[tcpBuffSize];/*received msg*/
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
  strcpy(isbn[7],"978858041-1\0");/*errado*/
  strcpy(isbn[8],"978858041-1\0");
  strcpy(isbn[9],"978857679-9\0");
  strcpy(isbn[10],"978857608-5\0");
  strcpy(isbn[11],"978853990-1\0");
  strcpy(isbn[12],"978857521-2\0");
  strcpy(isbn[13],"978850220-0\0");
  int i=0,random=0;
  int rc=0;
  max = recvfrom(sDesc,(void *)recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
  recvMsg[max]='\0';
  printf("%s",recvMsg);
  for(i=0;i<NUM_TESTES*NUM_OPCOES_MENU;i++){
    rc=0;
    gettimeofday(&tvini, NULL);/*marca envio*/
    printf("%d",msgT[i]);
    do{
      max = sendto(sDesc,my_itoa(msgT[i],10),strlen(my_itoa(msgT[i],10)),0,a->ai_addr,a->ai_addrlen);
      if(max<0){
	printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	close(sDesc);
	exit(1);
      }
    }while(max<1);
    if(msgT[i]==2 || msgT[i]==3 || msgT[i]==5 || msgT[i]==6){/*precisa de isbn*/
      max = recvfrom(sDesc,recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
      recvMsg[max]='\0';
      printf("%s",recvMsg);
      if(strcmp(recvMsg,TCP_MSG_ISBN_REQUIRED)==0){/*caso receba req. isbn*/
	random=rand()%13;
	do{
	  max = sendto(sDesc,isbn[random],strlen(isbn[random]),0,a->ai_addr,a->ai_addrlen);
	  if(max<0){
	    printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	    close(sDesc);
	    exit(1);
	  }
	}while(max<1);
	printf(" SEND %s+ ",isbn[random]);
	if(msgT[i]==5){/*precisa de senha && qntidade*/
	  max = recvfrom(sDesc,recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
	  recvMsg[max]='\0';
	  printf("%s",recvMsg);
	  if(strcmp(recvMsg,TCP_MSG_LOGIN_REQUIRED)==0){/*caso receba req. login*/
	    do{
	      max = sendto(sDesc,SENHA,strlen(SENHA),0,a->ai_addr,a->ai_addrlen);
	      if(max<0){
		printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
		close(sDesc);
		exit(1);
	      }
	    }while(max<1);
	    printf(" SEND %s+ ",SENHA);
	    max = recvfrom(sDesc,recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
	    recvMsg[max]='\0';
	    printf("%s",recvMsg);
	    if(strcmp(recvMsg,TCP_MSG_QTD_REQUIRED)==0){/*qntidade*/
	      random=rand()%1000;
	      do{
		max = sendto(sDesc,my_itoa(random,10),strlen(my_itoa(random,10)),0,a->ai_addr,a->ai_addrlen); 
		if(max<0){
		  printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
		  close(sDesc);
		  exit(1);
		}
	      }while(max<1);
	      printf(" SEND %s+ ",my_itoa(random,10));
	      max = recvfrom(sDesc,recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
	      recvMsg[max]='\0';
	    }/*qntd*/
	  }/*ogin*/
	}
	else{
	  max = recvfrom(sDesc,recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
	  recvMsg[max]='\0';
	}
      }/*isbn*/
      printf("@%s@",recvMsg);
    }
    else if(msgT[i]==4){
      do{
	max = recvfrom(sDesc,recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
	if(max<0){
	  printError((max==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	    close(sDesc);
	    exit(1);
	}
	recvMsg[max]='\0';
	printf("@%s@",recvMsg);
	if(rc==0){
	  int maxaux=0;
	  do{ 
	    maxaux=sendto(sDesc,TCP_MSG_ACK,strlen(TCP_MSG_ACK),0,a->ai_addr,a->ai_addrlen);/*send message*/
	    if(maxaux<0){
	      printError((maxaux==-1)?ERROR_SOCKET_SERVER_ERROR:ERROR_SOCKET_SERVER_CLOSED);
	      close(sDesc);
	      exit(1);
	    }
	  }while(maxaux<1);
	}
	rc+=max;
      }while(rc<tcpBuffSize);
      }
    else{
      max = recvfrom(sDesc,(void *)recvMsg,tcpBuffSize,0,a->ai_addr,&(a->ai_addrlen));
      recvMsg[max]='\0';
      printf("@%s@",recvMsg);
    }
    gettimeofday(&tvend, NULL);/*marca recebimento*/
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
