#ifndef _H_SERVER
#define _H_SERVER
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../common/error.h"
#include "../common/log.h"
#include "../common/tcp.h"
#include "../common/books.h"
#include "../common/archives.h"
#include "../common/avl.h"
#include "tcp_server.h"
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
