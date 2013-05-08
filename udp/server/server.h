#ifndef _H_SERVER
#define _H_SERVER
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../tcp/common/error.h"
#include "../../tcp/common/log.h"
#include "../../tcp/common/tcp.h"
#include "../../tcp/common/books.h"
#include "../../tcp/common/archives.h"
#include "../../tcp/common/avl.h"
#include "udp_server.h"
/**
 * funcao que chama o bind() para o endereco e faz o listen() no mesmo
 * e a funcao bindNlisten() faz chamada para receive_msg()
 * @param char *argv[]
 **/
void start_server(char *argv[]);

/**
 * @function : Funcao que carrega os livros para a memoria
 * @param struct avl **l : estrutura dos livros
 * @see books.h, avl.h, archives.h
 **/
void load_books(AVL *l);
#endif
