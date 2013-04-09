#include "client.h"

void start_client(char *argv[]){
  int socket;
  struct in_addr *addr = (struct in_addr *)malloc(sizeof(struct in_addr));
  addr->s_addr = inet_addr(argv[1]);
  socket = conn(*addr,atoi(argv[2]));
  send_msg(socket,*addr,atoi(argv[2]));
  close(socket);
  free(addr);
}
