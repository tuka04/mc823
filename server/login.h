#ifndef _H_LOGIN
#define _H_LOGIN

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SENHA "MC823"
#define MSG_SENHA_INVALIDA "Senha invalida!\n"

/**
 * Verifica se o login eh valido
 * @param char *senha
 * @return bool: true -> sucesso ; false -> senha invalida
 **/
bool doLogin(char *senha);

#endif
