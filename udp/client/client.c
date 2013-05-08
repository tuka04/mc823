#include "client.h"

void start_client(int argc, char *argv[]){
  struct in_addr *aux = (struct in_addr *)malloc(sizeof(struct in_addr));
  struct sockaddr_in *addr=(struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  struct addrinfo *ainfo=(struct addrinfo *)malloc(sizeof(struct addrinfo));
  memset(ainfo,0,sizeof(struct addrinfo));
  memset(addr,0,sizeof(struct sockaddr_in));
  memset(aux,0,sizeof(struct in_addr));
  addr->sin_family=AF_INET;
  addr->sin_port=htons(atoi(argv[2]));
  aux->s_addr=inet_addr(argv[1]);
  addr->sin_addr=*aux;
  ainfo->ai_addr=(SA*)addr;
  ainfo->ai_addrlen=(socklen_t)sizeof(struct sockaddr);
  int socket=openSocket(ainfo);
  if(argc==3)
    send_msg(socket,ainfo);
  else if(argc==4 && strcmp(argv[3],OPT_TESTE)==0){
    printf("%d %s %s",argc, argv[3],OPT_TESTE);
    build_teste(socket,ainfo);
  }
  close(socket);
  free(aux);
  free(addr);
  free(ainfo);
}
