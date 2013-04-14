#ifndef _H_TEMPO
#define _H_TEMPO

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "error.h"
#include "archives.h"
#define NUM_TESTES 100

typedef struct headTempo {
  char *medida;//nome da medida
  double media;//media
  double desvio;//devio padrao
  int length;//quantidade de dados
  struct tempo *dados;//tempo
}Media;

typedef struct tempo {
  double inicio;
  double fim;
  double decorrido;
  struct tempo *prox;
}Tempo;

/**
 * Inicia uma nova media
 * @param Media **m : estrutura com os dados e media
 * @param char *mname : nome da medida (arquivo a ser salvo)
 **/
void newMedia(Media **m, char *mname);
/**
 * Calcula a media e desvio a partir dos dados
 * @param Media **m : estrutura com os dados e media
 **/
void calcMedia(Media **m);
/**
 * Insere um tempo na estrutura
 * @param Media **m : estrutura com os dados e media
 * @param double i : tempo inicial
 * @param double f : tempo final
 **/
void insertTempo(Media **m,double i,double f);
/**
 * Le arquivos que contem os tempos registrados pelo teste
 * @see README
 * @param Media **m : estrutura com os dados e media
 * @param char *fname : caminho do arquivo
 **/
void readFileMedia(Media **m, char *fname);
/**
 * Escreve a media com devio em um arquivo dado pelo nome da medida
 * @param Media **m : estrutura com os dados e media
 **/
void writeFileMedia(Media **m);

#endif
