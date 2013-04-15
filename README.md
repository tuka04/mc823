mc823
=====

mc823 - Projeto 1 - Servido Concorrente TCP

#########
# Maiores informações sobre cada função utilizada no projeto veja os arquivo *.h
########

# Introducao: 
  O sistema pode ser utilizado de duas maneiras, a primeira para testes e a segunda para uso de uma "biblioteca em rede".

1) Compilar:
   Para compilar digite "make" (para recompilar digite "make clean && make")

2) Teste:
   Para rodar o teste digite "make test"
   * Opcional:
     ./main <porta> (iniciar o servidor)
     ./main <ip-servidor> <porta> -t 
 
3) Biblioteca em Rede:
   Para rodar o sistema digite "make start_server" (iniciar servidor) e "make start_client IP=<ip-servidor> " para inicar o cliente.
   * Opcional:
     ./main <porta> (inicar servidor)
     ./main <ip-servidor> <porta>

# Arquivo da livraria: ./common/livros/livros
 Esse arquivo contem os livros do sistema.
 *ATENCAO: Deve seguir o seguinte formato
 ISBN#Autores;#Titulo#Ano#Desc#Editora#Quantidade#