#include "client.h"

void start_client(int argc, char *argv[]){
  int socket;
  struct in_addr *addr = (struct in_addr *)malloc(sizeof(struct in_addr));
  addr->s_addr = inet_addr(argv[1]);
  socket = conn(*addr,atoi(argv[2]));
  if(argc==3)
    send_msg(socket,*addr,atoi(argv[2]));
  else if(argc==4 && strcmp(argv[3],OPT_TESTE)==0)
    build_teste(socket,*addr,atoi(argv[2]));
  close(socket);
  free(addr);
}
