#include "main.h"

int main(int argc, char *argv[]){
  if(argc<2){
    printError(ERROR_USAGE_MAIN);
    exit(1);
  }
  if(argc==2)
    start_server(argv);
  else
    start_client(argc, argv);
  return 0;
}
