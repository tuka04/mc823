#ifndef _H_CLIENT
#define _H_CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../common/error.h"
#include "../common/log.h"
#include "../common/tcp.h"
#include "tcp_client.h"

typedef struct client_ipv4 {
  in_addr_t addr;
  uint16_t port;
}cliente_ipv4;

typedef struct client_ipv6 {
  in_addr_t addr;
  uint32_t port;
}cliente_ipv6;
/**
 * funcao que inicia uma conexao, e mantem aberta, com um servidor
 * @param char *argv[]
 **/
void start_client(char *argv[]);

#endif
