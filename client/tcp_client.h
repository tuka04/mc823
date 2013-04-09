#ifndef _H_TCP_CLIENT
#define _H_TCP_CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../common/error.h"
#include "../common/log.h"
#include "../common/tcp.h"

/**
 * Funcao que realiza a conexao
 * Utiliza IPv4 (AF_INET) e SOCK_STREAM, ou seja, prove uma
 * conexao sequencial, confiavel, 2-way, e byte-stream .
 * @example
 * struct sockaddr_in {
 *   sa_family_t    sin_family; address family: AF_INET 
 *   in_port_t      sin_port;   port in network byte order 
 *   struct in_addr sin_addr;   internet address 
 * }
 * @return int sDesc : retorna o socket descriptor
 **/
int conn(struct in_addr addr, int port);
/**
 * Funcao que envia e recebe uma mensagem do servidor. TUDO EH GRAVADO NO LOG
 * @see log.h
 * @param int sDesc : descriptor de uma conexao socket
 **/
void send_msg(int sDesc,struct in_addr addr, int port);
#endif
