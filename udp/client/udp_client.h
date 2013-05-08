#ifndef _H_UDP_CLIENT
#define _H_UDP_CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../../tcp/common/error.h"
#include "../../tcp/common/log.h"
#include "../../tcp/common/tcp.h"
#include "../../tcp/common/archives.h"
#include "../../tcp/server/login.h"

/**
 * Funcao que abre um socket
 * Utiliza IPv4 (AF_INET) e SOCK_DGRAM, não orientado a conexão!
 * @example
 * struct sockaddr_in {
 *   sa_family_t    sin_family; address family: AF_INET 
 *   in_port_t      sin_port;   port in network byte order 
 *   struct in_addr sin_addr;   internet address 
 * }
 * @return int sDesc : retorna o socket descriptor
 **/
int openSocket(struct addrinfo *a);
/**
 * Funcao que envia a primeira msg ao servidor. Ha um numero maximo de tentativas
 * @see udp.h : UDP_MAX_RETRY
 * @see log.h
 * @param int sDesc : descriptor de um socket
 * @param struct addrinfo *a : info, tal como endereco do server
 **/
void first_msg(int s, AI *a);
/**
 * Funcao que envia ack ao servidor. Nao importa se chegou ou nao
 * @see log.h
 * @param int sDesc : descriptor de um socket
 * @param struct addrinfo *a : info, tal como endereco do server
 **/
void send_ack(int s, AI *a);
/**
 * Funcao que envia e recebe uma mensagem do servidor.
 * @see log.h
 * @param int sDesc : descriptor de um socket
 * @param struct addrinfo *a : info, tal como endereco do server
 **/
void send_msg(int sDesc,AI *a);
/**
 * Roda um teste juntamente ao server
 **/
void build_teste(int sDesc,AI *a);
/**
 * Gera um vetor com opcoes(int) para o teste.
 * @return int *msg;
 **/
int *getTesteMsg();

#endif
