#ifndef _H_TCP
#define _H_TCP

#define TCP_BUF_SIZE 1024 /*tamanho do buffer de escrita*/
#define TCP_MAX_CONN 50 /*maximo de conexoes que o server tcp aceitara*/
#define TCP_MSG_WELCOME "Welcome to Server\n"
#define TCP_MSG_BYE "Bye\n"
#define TCP_MSG_ACK "ACK\0"
#define TCP_MSG_COMMAND_NOT_FOUND "*ERRO*: Comando nao reconhecido\n"
#define TCP_MSG_NUM_NATURAL_REQUIRED "*ERRO*: A entrada deve ser um numero maior que zero\n"
#define TCP_MSG_SUCESS_EDIT "Campo editado com sucesso!\n"
#define TCP_MSG_LOGIN_REQUIRED "Senha : "
#define TCP_MSG_ISBN_REQUIRED "ISBN : " 
#define TCP_MSG_QTD_REQUIRED "Quantidade : " 
#define TCP_COMMAND_CLOSE_CONNECTION "\\q"
#define TCP_COMMAND_MENU "\\m"

#endif
