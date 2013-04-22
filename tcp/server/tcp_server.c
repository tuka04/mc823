#include "tcp_server.h"

void bindNlisten(int port, AVL *l){
  int sDesc;//socket descriptor
  int conn; //nova conexao de um cliente
  int started;//bind,listen
  char str[32];
  socklen_t len = sizeof(struct sockaddr);//evitar: error: conversion to non-scalar type requested
  struct sockaddr_in *s = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));//servidor
  struct sockaddr_in *c = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));//cliente
  struct in_addr *addr = (struct in_addr *)malloc(sizeof(struct in_addr));
  sDesc = socket(AF_INET,SOCK_STREAM,0);//IPv4 e TCP (sequencial,confiavel,2-way,byte-stream)
  if(sDesc<0){
    printError(ERROR_SOCKET);
    exit(1);
  }  
  memset(str,'\0',sizeof(str));
  memset(s,'0',sizeof(s));
  memset(addr,'0',sizeof(addr));//write zero-valued bytes
  addr->s_addr = htons(INADDR_ANY);//bind all local interfaces
  s->sin_family = AF_INET;//tcp
  s->sin_port = htons(port);//convert to network byte order
  s->sin_addr = *addr;
  started = bind(sDesc,(struct sockaddr*)s,len);//assigns the address specified to by addr to the socket 
  if(started<0){
    printError(ERROR_SOCKET_SERVER_BIND);
    close(sDesc);//lembrar de fechar o socket aberto
    exit(1);
  }
  started = listen(sDesc,TCP_MAX_CONN);
  if(started<0){
    printError(ERROR_SOCKET_SERVER_LISTEN);
    close(sDesc);//lembrar de fechar o socket aberto
    exit(1);
  }
  //  writeLog("TCP_SERVER::iniciado porta",my_itoa(port,10),"");
  //conexao? wait
  while(1){
    conn=accept(sDesc,(struct sockaddr *)c,&len);
    if(!fork()){
      close(sDesc);
      send_max_size(conn,l);
      receive_msg(conn,l);
    }
    else{
      close(conn);
    }
  };   
  close(sDesc);
  free(s);
  free(c);
  free(addr);
}

void receive_msg(int conn, AVL *l){
  char msg[TCP_BUF_SIZE];//msg transmit
  int b;//byte received
  bool transmit=false;
  double ini;
  int i=0;
  struct timeval tvini; //apenas assim para pegar microsegundos
  do{
    b = recv(conn,msg,TCP_BUF_SIZE,0);
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
    printf("%s\n",msg);
    if(strcmp(msg,TCP_MSG_ACK)==0){
      //do nothing
      memset(msg,'\0',TCP_BUF_SIZE);
    }
    else if(strlen(msg)>5){
      send_menu(conn,true,NULL);
    }
    else if(strcmp(msg,TCP_COMMAND_MENU)==0){
      send_menu(conn,false,NULL);
    }
    else if (strcmp(msg,TCP_COMMAND_CLOSE_CONNECTION) == 0){
      send(conn,TCP_MSG_BYE,strlen(TCP_MSG_BYE), 0);
    }
    else{
      ini=DOUBLE_MILHAO*(double)(tvini.tv_sec)+(double)(tvini.tv_usec);
      read_menu(conn,l,msg,ini);
    }
    transmit=true;
  }while(transmit);
  close(conn);
}

void send_menu(int conn,bool alert, TimeVal *tvend){
  char *msg[TAM_MENU];
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
  for(i=0;i<TAM_MENU;i++)
    tam+=strlen(msg[i]);
  if(alert)
    tam+=strlen(TCP_MSG_COMMAND_NOT_FOUND);
  char aux[tam];
  memset(aux,'\0',tam+1);
  if(alert){
    strcat(aux,TCP_MSG_COMMAND_NOT_FOUND);
    last=strlen(TCP_MSG_COMMAND_NOT_FOUND);
  }
  for(i=0;i<TAM_MENU;i++){
    for(j=0;j<strlen(msg[i]);j++){
      aux[last+j]=msg[i][j];
    }
    last+=j;
  }
  last=strlen(aux);
  if(tvend != NULL)
    gettimeofday(tvend, NULL);
  send(conn,aux,last, 0);//chegara em ordem, tcp
}

bool read_menu(int conn, AVL *l, char opt[], double ini){
  double end; 
  struct timeval tvend; //apenas assim para pegar microsegundos?
  int i=0;
  for(i=0;i<strlen(opt);i++){
    if(strcmp(opt,TCP_MSG_ACK)==0)
      return false;
    if(!isdigit(opt[i])){
      send_menu(conn,true,NULL);
      return false;
    }
  }
  i=atoi(opt);
  switch(i){
  case 1:
    send_all_ids(conn,l,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_1","a+",end-ini);
    return true;
  case 2:
    send_desc_byId(conn,l,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_2","a+",end-ini);
    return true;
  case 3:
    send_book_info(conn,l,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_3","a+",end-ini);
    return true;
  case 4:
    send_all_books_info(conn,l,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_4","a+",end-ini);
    return true;
  case 5:
    edit_estoque(conn,l,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_5","a+",end-ini);
    return true;
  case 6:
    send_estoque_byId(conn,l,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_6","a+",end-ini);
    return true;
  default:
    send_menu(conn,true,&tvend);
    end=DOUBLE_MILHAO*(double)(tvend.tv_sec)+(double)(tvend.tv_usec);
    writeDoubleToFile("./estat/tp_menu","a+",end-ini);
    return false;
  }
  return true;
}

void send_all_ids(int conn, AVL *l, TimeVal *tvend){
  int len=totISBNchar(*l);
  int tot=totElemAVL(*l);
  char *del="\n ISBN: ";
  char *ids=(char *)malloc(len+tot*strlen(del)+2);
  memset(ids,'\0',len+tot*strlen(del)+2);
  avlIdToStr(*l,&ids,del);
  strcat(ids,"\n");
  gettimeofday(tvend,NULL);
  send(conn,ids,strlen(ids),0);
  free(ids);
}

void send_desc_byId(int conn, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];//msg transmit
  send(conn,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0);
  int b=recv(conn,msg,TCP_BUF_SIZE,0);//isbn?
  msg[b] = '\0'; 
 printf("%s\n",msg);
  AVL_NO *livro=getAVLElemById(*l,msg);
  if(livro!=NULL){
    Livro *temp=(Livro *)livro->dado;
    char str[strlen(temp->desc)+4];
    memset(str,'\0',strlen(temp->desc)+4);
    strcat(str,temp->desc);
    strcat(str,"\n"); 
    gettimeofday(tvend,NULL);
    send(conn,str,strlen(str),0);
  } 
  else{
    gettimeofday(tvend,NULL);
    send(conn,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0);
  }
}

void send_book_info(int conn, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];//msg transmit
  send(conn,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0);
  int b=recv(conn,msg,TCP_BUF_SIZE,0);//isbn?
  msg[b] = '\0'; 
  AVL_NO *livro=getAVLElemById(*l,msg);
 printf("%s\n",msg);
  if(livro!=NULL){
    Livro *temp=(Livro *)livro->dado;
    char *str = bookNodeToStr(temp);
    gettimeofday(tvend,NULL);
    send(conn,str,strlen(str),0);
  } 
  else{
    gettimeofday(tvend,NULL);
    send(conn,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0);
  }
}

void send_all_books_info(int conn, AVL *l, TimeVal *tvend){
  int len=totAVLchar(*l);
  int tot=totElemAVL(*l);
  int bsize=getBookNodeSize();
  char *del="\n";
  int lentot=(len+tot*bsize+tot*strlen(del)*2+2);
  char *allin=(char *)malloc(lentot);
  memset(allin,'\0',lentot);
  avlToStr(*l,&allin,del);
  strcat(allin,"\n");
  gettimeofday(tvend,NULL);
  send(conn,allin,lentot,0);
  free(allin);
}

void edit_estoque(int conn, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];//msg transmit
  send(conn,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0);
  int b=recv(conn,msg,TCP_BUF_SIZE,0);//isbn?
  msg[b] = '\0'; 
 printf("%s\n",msg);
  AVL_NO *livro=getAVLElemById(*l,msg);
  if(livro!=NULL){
    send(conn,TCP_MSG_LOGIN_REQUIRED,strlen(TCP_MSG_LOGIN_REQUIRED),0);
    b=recv(conn,msg,TCP_BUF_SIZE,0);//senha?
    msg[b] = '\0';
 printf("%s\n",msg);
    if(doLogin(msg)){
      send(conn,TCP_MSG_QTD_REQUIRED,strlen(TCP_MSG_QTD_REQUIRED),0);
      b=recv(conn,msg,TCP_BUF_SIZE,0);//senha?
      msg[b] = '\0';
 printf("%s\n",msg);
      int q=atoi(msg);
      if(q>=0){
	Livro *aux = (Livro *)livro->dado;
	aux->estoque=q;
	livro->dado = (void *)aux;
	gettimeofday(tvend,NULL);
	send(conn,TCP_MSG_SUCESS_EDIT,strlen(TCP_MSG_SUCESS_EDIT),0);
      }
      else{
	gettimeofday(tvend,NULL);
	send(conn,TCP_MSG_NUM_NATURAL_REQUIRED,strlen(TCP_MSG_NUM_NATURAL_REQUIRED),0);
      }
    }
    else{
      gettimeofday(tvend,NULL);
      send(conn,MSG_SENHA_INVALIDA,strlen(MSG_SENHA_INVALIDA),0);
    }
  } 
  else{
    gettimeofday(tvend,NULL);
    send(conn,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0);
  }    
}

void send_estoque_byId(int conn, AVL *l, TimeVal *tvend){
  char msg[TCP_BUF_SIZE];//msg transmit
  send(conn,TCP_MSG_ISBN_REQUIRED,strlen(TCP_MSG_ISBN_REQUIRED),0);
  int b=recv(conn,msg,TCP_BUF_SIZE,0);//isbn?
  msg[b] = '\0'; 
  printf("%s\n",msg);
  AVL_NO *livro=getAVLElemById(*l,msg);
  if(livro!=NULL){
    Livro *aux = (Livro *)livro->dado;
    gettimeofday(tvend,NULL);
    send(conn,my_itoa(aux->estoque,10),strlen(my_itoa(aux->estoque,10)),0);
  } 
  else{
    gettimeofday(tvend,NULL);
    send(conn,BOOK_NOT_FOUND,strlen(BOOK_NOT_FOUND),0);
  }
}

void send_max_size(int conn, AVL *l){
  int len=totAVLchar(*l);
  int tot=totElemAVL(*l);
  int bsize=getBookNodeSize();
  char *del="\n";
  int lentot=(len+tot*bsize+tot*strlen(del)*2+2);
  char str[10];
  memset(str,'\0',10);
  memmove(str,my_itoa(lentot,10),strlen(my_itoa(lentot,10)));
  send(conn,str,strlen(str),0);
}
