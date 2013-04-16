include Makefile.inc

DIRS	= common client server
BIN	= main
BIN_TESTE = media
OBJS	= main.o common/common.o common/error.o common/archives.o common/log.o
OBJS_TESTE = common/tempo.o common/archives.o
OBJLIBS	= libcommon.a libserver.a libclient.a
LIBS	= -L. -lm -lcommon -lserver -lclient
PORTA = 2929
DDOT = :

all : $(BIN) $(BIN_TESTE)

$(BIN) : main.o $(OBJLIBS)
	$(ECHO) $(LD) -o $(BIN) $(OBJS) $(LIBS)  $<
	$(LD) -o $(BIN) $(OBJS) $(LIBS)

$(BIN_TESTE) : common/tempo.o $(OBJLIBS)
	$(ECHO) $(LD) -o $(BIN_TESTE) $(OBJS_TESTE) $(LIBS)  $<
	$(LD) -o $(BIN_TESTE) $(OBJS_TESTE) $(LIBS)

libcommon.a : force_look
	$(ECHO) looking into common : $(MAKE) $(MFLAGS)
	cd common; $(MAKE) $(MFLAGS)

libserver.a : force_look
	$(ECHO) looking into server : $(MAKE) $(MFLAGS)
	cd server; $(MAKE) $(MFLAGS)

libclient.a : force_look
	$(ECHO) looking into client : $(MAKE) $(MFLAGS)
	cd client; $(MAKE) $(MFLAGS)

clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(BIN) $(OBJS) $(OBJLIBS) *~ estat/* output
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

force_look :
	true
test :
	$(ECHO) "Executando teste...aguarde"	
	./main $(IP) $(PORTA) -t > output
	$(ECHO) "Executando media"
	./media
	$(ECHO) "****** Tempo de Processamento ******"
	-for i in 1 2 3 4 5 6; do ( $(ECHO) "Menu: $$i" && cat estat/mtp_$$i ); done
	$(ECHO) "****** Tempo Total *****************"
	-for i in 1 2 3 4 5 6; do ( $(ECHO) "Menu: $$i" && cat estat/mtt_$$i ); done
start_server : 
	$(ECHO) "Iniciando servidor: porta " $(PORTA)
	./main $(PORTA)
	$(ECHO) "Servidor iniciado! Aguardando clientes na porta: " $(PORTA)

start_client :
	$(ECHO) "Iniciando cliente: " $(IP) $(DDOT) $(PORTA)	
	./main $(IP) $(PORTA)
	$(ECHO) "Conexao finalizada " $(IP) $(DDOT) $(PORTA)	
