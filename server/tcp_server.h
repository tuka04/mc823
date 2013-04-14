#ifndef _H_TCP_SERVER
#define _H_TCP_SERVER
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "../common/error.h"
#include "../common/log.h"
#include "../common/tcp.h"
#include "../common/avl.h"
#include "../common/common.h"
#include "login.h"
#define TAM_MENU 8

typedef struct timeval TimeVal;
/**
 * Inicia um servidor socker na porta especificada como parametro.
 * Faz o bind e listen na porta e aguarda o contato de algum cliente.
 * @param int sDesc : descriptor de uma conexao socket
 * @param AVL *l : estrutura com info dos livros
 **/
void bindNlisten(int port, AVL *l);
/**
 * Funcao que envia e recebe uma mensagem do cliente. TUDO EH GRAVADO NO LOG
 * @see log.h
 * @see bindNlisten (accept)
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 **/
void receive_msg(int conn, AVL *l);

/**
 * Funcao que envia o menu para os clientes que acabaram de se conectar
 * ATENCAO, funcao boa para TCP, uma vez que, espera-se que o protocolo 
 * envie/receba ordenado, confiavel.
 * @see bindNlisten (accept)
 * @param int conn: connection accept
 * @param bool alerta : caso true concatena uma msg(TCP_MSG_COMMAND_NOT_FOUND) para cliente
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_menu(int conn, bool alert, TimeVal *tvend);

/**
 * Funcao que le e executa opcao de menu digitada pelo cliente.
 * @see bindNlisten
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param char opt[] ; string digitada pelo cliente
 * @param double ini : inicio do tempo de execucao
 **/
bool read_menu(int conn, AVL *l, char opt[], double ini);
/**
 * @function : Envia uma string para o cliente contendo todos os ids, formatados
 * para output no cliente.
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_all_ids(int conn, AVL *l, TimeVal *tvend);
/**
 * @function : Envia uma string para o cliente contendo a descricao, formatados
 * para output no cliente.
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_desc_byId(int conn, AVL *l, TimeVal *tvend);
/**
 * @function : Envia uma string para o cliente contendo a informacao total de um livro, formatados
 * para output no cliente.
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_book_info(int conn, AVL *l, TimeVal *tvend);
/**
 * Envia todas as informacoes de todos os livros
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_all_books_info(int conn, AVL *l, TimeVal *tvend);
/**
 * @function : Permite a edicao do estoque
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void edit_estoque(int conn, AVL *l, TimeVal *tvend);
/**
 * @function : Envia a quantidade em estoque de um livro para o cliente
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 * @param TimeVal *tvend : para calculo do tempo de processamento da requisicao
 **/
void send_estoque_byId(int conn, AVL *l, TimeVal *tvend);
/**
 * Envia o maximo de bytes, ou seja, o numero de bytes da maior msg
 * (NAO ESTA INCLUIDO NO TESTE DE PERFORMANCE)
 * @param int conn: connection accept
 * @param AVL *l : estrutura com info dos livros
 **/
void send_max_size(int conn, AVL *l);

#endif
