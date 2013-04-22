#ifndef _H_LOG
#define _H_LOG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "error.h" 
#define LOG_FILE "./log.txt"

/**
 * Escreve em um arquivo de log. Os parametros sao o titulo e mensagem a ser
 * escriva. Formato de escrita eh: title (time): msg
 * @param char *title
 * @param char *msg
 **/
void writeLog(char *title, char *msg, char *extra);
/**
 * Escreve um double em um determinado arquivo
 * @param char *file : caminho do arquivo
 * @param char *mode : modo de abertura do arquivo (a+,r+,r,w..)
 * @param double num : numero a ser escrito
 **/
void writeDoubleToFile(char *file,char *mode,double num);
#endif
