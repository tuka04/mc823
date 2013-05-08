#include "udp_server.h"
/**
 * NAO deve aceitar conexoes concorrentes
 * SERVIDOR UDP ITERATIVO
 **/
void init(int port, AVL *l){
  int sDesc;/*socket descriptor*/
  int started;/*bind,listen*/
  char *str=(char *)malloc(sizeof(char)*TCP_BUF_MIN_SIZE+1);
  SA_IN *c = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));/*cliente*/
  SA_IN *s = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));/*servidor*/
  struct in_addr *addr = (struct in_addr *)malloc(sizeof(struct in_addr));
  AI *ainfo=(struct addrinfo *)malloc(sizeof(struct addrinfo));/*addinfo*/
  /*zerando espaco da mem*/
  memset(ainfo,0,sizeof(struct addrinfo));
  memset(str,'\0',sizeof(char)*TCP_BUF_MIN_SIZE+1);
  /*atribuindo ainfo*/
  ainfo->ai_family=AF_INET;/*IPv4*/
  /* Usa-se datagram ao invez de byte-stream do TCP*/
  ainfo->ai_socktype=SOCK_DGRAM;/*upd entao, datagram*/
  ainfo->ai_protocol=IPPROTO_UDP;
  /*abrindo socket*/
  sDesc = socket(ainfo->ai_family,ainfo->ai_socktype,ainfo->ai_protocol);
  if(sDesc<0){/*erro?*/
    printError(ERROR_SOCKET);/*print erro*/
    exit(1);/*saida com erro*/
  }  
  /*zerar mem*/
  memset(addr,0,sizeof(addr));
  memset(s,0,sizeof(s));
  memset(c,0,sizeof(c));
  addr->s_addr = htonl(INADDR_ANY);/*bind all local interfaces*/
  s->sin_family = AF_INET;/*ipv4*/
  s->sin_port = htons(port);/*convert to network byte order*/
  s->sin_addr = *addr;/*endereco*/
  ainfo->ai_addr=(SA*)s;/*atribuindo servidor a info de end.*/
  ainfo->ai_addrlen=(socklen_t)sizeof(struct sockaddr_in);/*tam do addr*/
  /*atribuindo endereco ao socket_server*/
  started = bind(sDesc,ainfo->ai_addr,ainfo->ai_addrlen);
  if(started<0){/*erro?*/
    printError(ERROR_SOCKET_SERVER_BIND);/*imprime erro*/
    close(sDesc);/*fechar o socket aberto*/
    exit(1);/*saida com erro*/
  }
  /*escreve log*/
  if(LOG) writeLog("UDP_SERVER::iniciado porta",my_itoa(port,10),"");
  /*conexao? NAO ha o accept encontrado no tcp -> O.C.*/
  /*portanto vamos esperar um contato do cliente, assim gravar seu endereco*/
  receive_ack(c,sDesc);
  send_max_size(c,sDesc,l);/*envia tam da maior msg a ser enviada*/
  receive_ack(c,sDesc);
  while(1){
    // send_menu(c,sDesc,false,NULL);
    receive_msg(c,sDesc,l);/*comeca as requisicoes do sistema de livraria*/
  }   
  /*fechar o socket aberto*/
  close(sDesc);
  /*limpar mem*/
  free(str);
  free(c);
  free(addr);
  free(ainfo);
}
void receive_ack(SA_IN *c, int s){
  int len=strlen(TCP_MSG_ACK)*sizeof(char);
  char *str=(char *)malloc(len);
  unsigned int clilen=sizeof(struct sockaddr_in);
  memset(str,'\0',len);
  int max=recvfrom(s,str,len,0,(SA*)c,&clilen);
  if(max<0){/*erro?*/
    printError(ERROR_SOCKET_RCV_MSG);/*imprime erro*/
    close(s);/*fechar o socket aberto*/
    exit(1);/*saida com erro*/
  }
  if(max<len)
    str[max]='\0';/*sem erro, limit string*/
  if(LOG) writeLog("UDP_SERVER::rcv",inet_ntoa(c->sin_addr),str);
}
void receive_msg(SA_IN *c, int s, AVL *l){
  char msg[TCP_BUF_SIZE];/*msg transmit*/
  int b;/*byte received*/
  bool transmit=false;
  double ini;
  unsigned int len=sizeof(struct sockaddr_in);
  int i=0;
  struct timeval tvini; /*apenas assim para pegar microsegundos*/
  do{
    if(LOG) writeLog("UDP_SERVER::wainting menu","","");
    memset(msg,'\0',TCP_BUF_SIZE);
    b = recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&len);
    if(LOG)
      writeLog("UDP_SERVER::rcv",msg,"");
    gettimeofday(&tvini, NULL); 
    msg[b] = '\0';
    transmit=false;
    for(i=0;i<b;i=i+3){
      if(b-i >= 3){
	if(msg[i]!='A' || msg[i+1]!='C' || msg[i+2] != 'K'){
	  transmit=false;
	  i=b;
	}
	else{
	  transmit=true;
	}
      }
      else{
	transmit=false;
	i=b;
      }
    }
    if(transmit){
      for(i=3;i<TCP_BUF_SIZE;i++)
	msg[i]='\0';
    }
    if(strcmp(msg,TCP_MSG_ACK)==0){
      /*do nothing*/
      memset(msg,'\0',TCP_BUF_SIZE);
    }
    else if(strlen(msg)>5){
      send_menu(c,s,true,NULL);
    }
    else if(strcmp(msg,TCP_COMMAND_MENU)==0){
      send_menu(c,s,false,NULL);
    }
    else if (strcmp(msg,TCP_COMMAND_CLOSE_CONNECTION) == 0){
      sendto(s,TCP_MSG_BYE,strlen(TCP_MSG_BYE),0,(SA*)c,sizeof(c));
    }
    else{
      ini=DOUBLE_MILHAO*(double)(tvini.tv_sec)+(double)(tvini.tv_usec);
      read_menu(c,s,l,msg,ini);
    }
    transmit=true;
  }while(transmit);
}

void send_menu(SA_IN *c, int s,bool alert, TimeVal *tvend){
  char *msg[TAM_MENU];/*texto do menu*/
  msg[0] = "*** MENU (tecle \\m para visualizar o menu) *********\n\0";
  msg[1] = "*** (1) Listar ISBN                                *\n\0";
  msg[2] = "*** (2) Ver descricacao por ISBN (entrada: ISBN)   *\n\0";
  msg[3] = "*** (3) Ver info. completa (entrada: ISBN)         *\n\0";
  msg[4] = "*** (4) Ver info. completa (todos os livros)       *\n\0";
  msg[5] = "*** (5) Alterar estoque (apenas adm)               *\n\0";
  msg[6] = "*** (6) Ver quantidade em estoque (entrada: ISBN)  *\n\0";
  msg[7] = "*** (\\q) Fechar conexao e sair                     *\n\0";
  msg[8] = "****************************************************\n\0";
  int i=0,j=0;
  int last=0,tam=0;
  unsigned int clilen=sizeof(struct sockaddr_in);
  for(i=0;i<TAM_MENU;i++)/*contando tamanho da msg a ser criada*/
    tam+=strlen(msg[i]);
  if(alert)/*possui msg de alerta?*/
    tam+=strlen(TCP_MSG_COMMAND_NOT_FOUND);/*entao soma ao tamanho*/
  char aux[tam];/*string auxiliar, com tamanho calculado*/
  memset(aux,'\0',tam+1);/*zerando auxiliar*/
  if(alert){/*tem alerta?*/
    strcat(aux,TCP_MSG_COMMAND_NOT_FOUND);/*inicio da criacao da msg*/
    last=strlen(TCP_MSG_COMMAND_NOT_FOUND);/*marcando ultimo caracter da msg*/
  }
  for(i=0;i<TAM_MENU;i++){
    for(j=0;j<strlen(msg[i]);j++){
      aux[last+j]=msg[i][j];/*se nao a alerta last=0*/
    }
    last+=j;
  }
  last=strlen(aux);/*tam da msg a ser enviada*/
  if(tvend != NULL)/*marcar fim?*/
    gettimeofday(tvend, NULL);/*marcando tempo*/
  if(LOG)
    writeLog("UDP_SERVER::sending menu",aux,"");
  int max = 0;
  do{
    max=sendto(s,aux,last,0,(SA*)c,clilen);/*envia menu*/
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*envie pelo menos 1byte, evitar bloqueio*/
}

bool read_menu(SA_IN *c, int s, AVL *l, char opt[], double ini){
  double end; /*marcar tempo*/
  struct timeval tvend; /*marcar tempo*/
  int i=0;
  for(i=0;i<strlen(opt);i++){/*verifica a msg recebida*/
    if(strcmp(opt,TCP_MSG_ACK)==0)/*eh ACK?*/
      return false;/*do nothing*/
    if(!isdigit(opt[i])){/*nao eh digito?*/
      send_menu(c,s,true,NULL);/*envia menu com alerta*/
      return false;
    }
  }
  i=atoi(opt);/*ja foi verificado que eh digito*/
  switch(i){/*entao qual eh a opcao?*/
  case 1:
    send_all_ids(c,s,l,&tvend);/*envie todos os ids*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*marcar tempo*/
    writeDoubleToFile("./estat/tp_1","a+",end-ini);/*escreve estatistica*/
    return true;/*sucesso*/
  case 2:
    send_desc_byId(c,s,l,&tvend);/*envie descricao, mas antes requisite isbn*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*tempo*/
    writeDoubleToFile("./estat/tp_2","a+",end-ini);/*escreve estatistica*/
    return true;/*sucesso*/
  case 3:
    send_book_info(c,s,l,&tvend);/*pede isbn, envia info do livro*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*tempo*/
    writeDoubleToFile("./estat/tp_3","a+",end-ini);/*escreve estatistica*/
    return true;/*sucesso*/
  case 4:
    send_all_books_info(c,s,l,&tvend);/*todas as infos, maior msg*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*tempo*/
    writeDoubleToFile("./estat/tp_4","a+",end-ini);/*escreve estatistica*/
    return true;/*sucesso*/
  case 5:
    edit_estoque(c,s,l,&tvend);/*editar, dado isbn, senha e quantidade*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*tempo*/
    writeDoubleToFile("./estat/tp_5","a+",end-ini);/*escreve estatistica*/
    return true;/*sucesso*/
  case 6:
    send_estoque_byId(c,s,l,&tvend);/*envia estoque dado isbn*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*tempo*/
    printf("escrevendo 6");
    writeDoubleToFile("./estat/tp_6","a+",end-ini);/*escreve estatistica*/
    return true;/*sucesso*/
  default:
    send_menu(c,s,true,&tvend);/*reenvia menu*/
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);/*tempo*/
    writeDoubleToFile("./estat/tp_menu","a+",end-ini);/*estatistica*/
    return false;/*sem sucesso*/
  }
  return true;/*sucesso*/
}

void send_all_ids(SA_IN *c, int s, AVL *l, TimeVal *tvend){
  int len=totISBNchar(*l);/*tamanho da msg*/
  int tot=totElemAVL(*l);/*tamanho da msg*/
  char *del="\n ISBN: ";/*delimitador*/
  char *ids=(char *)malloc(len+tot*strlen(del)+2);/*alocando mem*/
  unsigned int clilen=sizeof(struct sockaddr_in);/*udp soh funciona assim*/
  memset(ids,'\0',len+tot*strlen(del)+2);/*zerando msg*/
  avlIdToStr(*l,&ids,del);/*convertendo avl para string*/
  strcat(ids,"\n");/*concatenando \n*/
  gettimeofday(tvend,NULL);/*marcar tempo (tempo de processamento)*/
  int max =0;
  do{
    max=sendto(s,ids,strlen(ids),0,(SA*)c,clilen);/*envia msg*/
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  if(LOG) writeLog("UDP_SERVER::send ISBN LIST",ids,"");
  free(ids);/*free allocated mem*/
}

void send_desc_byId(SA_IN *c, int s, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];/*msg transmited*/
  unsigned int clilen=sizeof(struct sockaddr_in);/*cliente len*/
  int max=0;
  do{
    max=sendto(s,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0,(SA*)c,clilen);/*requisicao do isbn*/
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  int b=recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&clilen);/*isbn?*/
  msg[b] = '\0';/*marcando fim da msg*/
  AVL_NO *livro=getAVLElemById(*l,msg);/*escolhe elem pelo id=isbn*/
  if(livro!=NULL){/*isbn valido?*/
    Livro *temp=(Livro *)livro->dado;/*casting dos dados*/
    char str[strlen(temp->desc)+4];/*msg a ser enviada*/
    memset(str,'\0',strlen(temp->desc)+4);/*zerando mem*/
    strcat(str,temp->desc);/*concatenando descricao a msg*/
    strcat(str,"\n");/*concatenando \n*/
    gettimeofday(tvend,NULL);/*marcando tempo de processamento*/
    do{
      max=sendto(s,str,strlen(str),0,(SA*)c,clilen);/*envia msg*/
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  } 
  else{/*isbn invalido*/
    gettimeofday(tvend,NULL);/*marcando tempo de processamento*/
    /*envia msg de livro nao encontrado*/
    do{
      max=sendto(s,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0,(SA*)c,clilen);
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  }
}

void send_book_info(SA_IN *c, int s, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];/*msg transmited*/
  unsigned int len=sizeof(struct sockaddr_in);/*cliente len*/
  int max=0;
  do{
    max=sendto(s,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0,(SA*)c,len);/*requisicao do isbn*/
  if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  int b=recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&len);/*isbn?*/
  msg[b] = '\0'; /*marcando fim da msg*/
  AVL_NO *livro=getAVLElemById(*l,msg);/*escolhe elem pelo id=isbn*/
  if(livro!=NULL){/*isbn valido?*/
    Livro *temp=(Livro *)livro->dado;/*casting dos dados*/
    char *str = bookNodeToStr(temp);/*msg a ser enviada*/
    gettimeofday(tvend,NULL);/*marcando tempo de processamento*/
    do{
      max=sendto(s,str,strlen(str),0,(SA*)c,len);/*envia msg*/
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  } 
  else{/*isbn invalido*/
    gettimeofday(tvend,NULL);/*marcando tempo de processamento*/
    /*envia msg de livro nao encontrado*/
    do{
      max=sendto(s,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0,(SA*)c,len);
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  }
}

void send_all_books_info(SA_IN *c, int s, AVL *l, TimeVal *tvend){
  int len=totAVLchar(*l);/*calculo da msg tam*/
  int tot=totElemAVL(*l);/*calculo da msg tam*/
  int bsize=getBookNodeSize();/*calculo da msg tam*/
  char *del="\n";/*delimitador*/
  int lentot=(len+tot*bsize+tot*strlen(del)*2+2);/*tamanho total*/
  char *allin=(char *)malloc(lentot);/*alocacao de memoria para msg*/
  memset(allin,'\0',lentot);/*zerando*/
  avlToStr(*l,&allin,del);/*converte toda estrutura avl para string*/
  strcat(allin,"\n");/*concatena \n*/
  gettimeofday(tvend,NULL);/*tempo de processamento*/
  unsigned int clilen=sizeof(struct sockaddr_in);/*cliente len*/
  int max=0;
  do{
    max=sendto(s,allin,lentot,0,(SA*)c,clilen);/*envia msg*/
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  free(allin);/*free mem*/
}

void edit_estoque(SA_IN *c, int s, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];/*msg transmited*/
  unsigned int len=sizeof(struct sockaddr_in);/*cliente len*/
  /*requisicao do isbn*/
  int max=0;
  do{
    max=sendto(s,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0,(SA*)c,len);
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  int b=recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&len);/*isbn?*/
  msg[b] = '\0';/*\0 ao fim da msg*/
  AVL_NO *livro=getAVLElemById(*l,msg);/*buscando isbn na estrutura*/
  if(livro!=NULL){/*isbn valido?*/
    /*entao requisicao da senha*/
    do{
      max=sendto(s,TCP_MSG_LOGIN_REQUIRED,strlen(TCP_MSG_LOGIN_REQUIRED),0,(SA*)c,len);
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
    b=recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&len);/*senha?*/
    msg[b] = '\0';/*limitando msg*/
    if(doLogin(msg)){/*reliza login, senha valida?*/
      /*entao requisicao de quantidade*/
      do{
	max=sendto(s,TCP_MSG_QTD_REQUIRED,strlen(TCP_MSG_QTD_REQUIRED),0,(SA*)c,len);
	if(max==-1){
	  printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	  close(s);/*fechar o socket aberto*/
	  exit(1);/*saida com erro*/
	}
      }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
      b=recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&len);/*quantidade*/
      msg[b] = '\0';/*limit msg*/
      int q=atoi(msg);/*convert str to in*/
      if(q>=0){/*eh um numero natural?*/
	Livro *aux = (Livro *)livro->dado;/*casting do dado*/
	aux->estoque=q;/*atualiza memoria*/
	livro->dado = (void *)aux;/*dado alterado*/
	gettimeofday(tvend,NULL);/*tempo de proc.*/
	/*editado com sucesso*/
	do{
	  max=sendto(s,TCP_MSG_SUCESS_EDIT,strlen(TCP_MSG_SUCESS_EDIT),0,(SA*)c,len);
	  if(max==-1){
	    printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	    close(s);/*fechar o socket aberto*/
	    exit(1);/*saida com erro*/
	  }
	}while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
      }
      else{
	gettimeofday(tvend,NULL);/*tempo de proc.*/
	/*erro na quantidade*/
	do{
	  max=sendto(s,TCP_MSG_NUM_NATURAL_REQUIRED,strlen(TCP_MSG_NUM_NATURAL_REQUIRED),0,(SA*)c,len);
	  if(max==-1){
	    printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	    close(s);/*fechar o socket aberto*/
	    exit(1);/*saida com erro*/
	  }
	}while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
      }
    }
    else{
      gettimeofday(tvend,NULL);/*tempo de processamento*/
      /*senha invalida*/
      do{
	max=sendto(s,MSG_SENHA_INVALIDA,strlen(MSG_SENHA_INVALIDA),0,(SA*)c,len);
	if(max==-1){
	  printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	  close(s);/*fechar o socket aberto*/
	  exit(1);/*saida com erro*/
	}
      }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
    }
  } 
  else{
    gettimeofday(tvend,NULL);/*tp*/
    /*isbn invalido*/
    do{
      max=sendto(s,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0,(SA*)c,len);
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  }    
}

void send_estoque_byId(SA_IN *c, int s, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];/*msg transmited*/
  unsigned int len=sizeof(struct sockaddr_in);/*cliente len*/
  int max=0;
  /*requisicao pelo isbn*/
  do{
    max=sendto(s,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0,(SA*)c,len);
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  int b=recvfrom(s,msg,TCP_BUF_SIZE,0,(SA*)c,&len);/*isbn?*/
  msg[b] = '\0'; /*limit msg*/
  AVL_NO *livro=getAVLElemById(*l,msg);/*busca por isbn*/
  if(livro!=NULL){/*achou?*/
    /*entao seleciona livro*/
    Livro *aux = (Livro *)livro->dado;
    gettimeofday(tvend,NULL);/*tp*/
    /*envia msg*/
    do{
      max=sendto(s,my_itoa(aux->estoque,10),strlen(my_itoa(aux->estoque,10)),0,(SA*)c,len);
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  } 
  else{/*isbn invalido*/
    gettimeofday(tvend,NULL);/*tp*/
    do{
      max=sendto(s,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0,(SA*)c,len);/*nao achou*/
      if(max==-1){
	printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
	close(s);/*fechar o socket aberto*/
	exit(1);/*saida com erro*/
      }
    }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
  }
}

void send_max_size(struct sockaddr_in *c, int s, AVL *l){
  int len=totAVLchar(*l);/*tam da msg*/
  int tot=totElemAVL(*l);/*tam da msg*/
  int bsize=getBookNodeSize();/*tam da msg*/
  char *del="\n";/*delimitador*/
  int lentot=(len+tot*bsize+tot*strlen(del)*2+2);/*calculo do tam total*/
  char str[10];/*msg*/
  unsigned int clilen=sizeof(struct sockaddr_in);
  memset(str,'\0',10);/*zerando mem*/
  /*convertendo int par string*/
  memmove(str,my_itoa(lentot,10),strlen(my_itoa(lentot,10)));
  /*envia msg*/
  if(LOG)
    writeLog("UDP_SERVER::send",str,"");
  len = strlen(str);
  int max=0;
  do{
    max=sendto(s,str,len,0,(struct sockaddr*)c,clilen);
    if(max==-1){
      printError(ERROR_SOCKET_SERVER_ERROR);/*imprime erro*/
      close(s);/*fechar o socket aberto*/
      exit(1);/*saida com erro*/
    }
  }while(max<1);/*enviar pelo menos 1 byte, evitar bloqueio*/
}
