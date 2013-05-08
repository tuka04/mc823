#include "login.h"

bool doLogin(char *senha){
  if(memcmp(senha,SENHA,strlen(senha))!=0)
    return false;
  return true;
}
