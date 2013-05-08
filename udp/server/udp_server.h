#ifndef _H_UDP_SERVER
#define _H_UDP_SERVER
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "../../tcp/common/error.h"
#include "../../tcp/common/log.h"
#include "../../tcp/common/tcp.h"
#include "../../tcp/common/avl.h"
#include "../../tcp/common/common.h"
#include "login.h"
#define TAM_MENU 9


/**
 * Inicia um servidor socker na porta especificada como parametro.
 * Faz o bind e listen na porta e aguarda o contato de algum cliente.
 * @param int sDesc : descriptor de uma conexao socket
 * @param AVL *l : estrutura com info dos livros
 **/
void init(int port, AVL *l);
/**
 * Funcao que recebe uma mensagem (geralmente ACK) de um cliente
 * @see init : com datagram e nao orientado a conexao
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 **/
void receive_ack(SA_IN *c, int s);
/**
 * Funcao que envia e recebe uma mensagem do cliente. TUDO EH GRAVADO NO LOG
 * @see log.h
 * @see init : com datagram e nao orientado a conexao
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 **/
void receive_msg(struct sockaddr_in *c, int s, AVL *l);

/**
 * Funcao que envia o menu para os clientes que acabaram de se conectar
 * ATENCAO, funcao boa para TCP, uma vez que, espera-se que o protocolo 
 * envie/receba ordenado, confiavel.
 * @see bindNlisten (accept)
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param bool alerta : caso true concatena uma msg(TCP_MSG_COMMAND_NOT_FOUND) para cliente
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_menu(struct sockaddr_in *c, int s, bool alert, TimeVal *tvend);

/**
 * Funcao que le e executa opcao de menu digitada pelo cliente.
 * @see bindNlisten
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param char opt[] ; string digitada pelo cliente
 * @param double ini : inicio do tempo de execucao
 **/
bool read_menu(struct sockaddr_in *c, int s, AVL *l, char opt[], double ini);
/**
 * @function : Envia uma string para o cliente contendo todos os ids, formatados
 * para output no cliente.
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_all_ids(struct sockaddr_in *c, int s, AVL *l, TimeVal *tvend);
/**
 * @function : Envia uma string para o cliente contendo a descricao, formatados
 * para output no cliente.
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_desc_byId(struct sockaddr_in *c, int s, AVL *l, TimeVal *tvend);
/**
 * @function : Envia uma string para o cliente contendo a informacao total de um livro, formatados
 * para output no cliente.
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_book_info(struct sockaddr_in *c, int s, AVL *l, TimeVal *tvend);
/**
 * Envia todas as informacoes de todos os livros
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_all_books_info(struct sockaddr_in *c, int s, AVL *l, TimeVal *tvend);
/**
 * @function : Permite a edicao do estoque
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void edit_estoque(struct sockaddr_in *c, int s, AVL *l, TimeVal *tvend);
/**
 * @function : Envia a quantidade em estoque de um livro para o cliente
 * @param struct sockaddr_in *c: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_estoque_byId(struct sockaddr_in *c, int s, AVL *l, TimeVal *tvend);
/**
 * Envia o maximo de bytes, ou seja, o numero de bytes da maior msg
 * (NAO ESTA INCLUIDO NO TESTE DE PERFORMANCE)
 * @param struct sockaddr_in *a: endereco do cliente;
 * @param int s : socket descriptor @see init(int port, AVL *l);
 * @param AVL *l : estrutura com info dos livros;
 **/
void send_max_size(struct sockaddr_in *c, int s, AVL *l);

#endif
