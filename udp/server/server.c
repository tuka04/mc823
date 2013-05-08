#include "server.h"
void start_server(char *argv[]){
  AVL l;
  newAVLTree(&l);
  load_books(&l);
  init(atoi(argv[1]),&l);
  //  freeAVL(l);
}
     
void load_books(AVL *l){
  readFileBooks(openFile(FILE_BOOK,"r"),l);
}
