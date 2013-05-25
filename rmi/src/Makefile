include Makefile.inc

DIRS	= common biblioteca client server
STUB = stub
STUB_FILE = biblioteca.BibliotecaImpl
ESTAT = estat
BIN_DIR = ../bin/
all : $(DIRS) $(STUB) $(ESTAT)

common : force_look
	$(ECHO) looking into common : $(MAKE) $(MFLAGS)
	cd common; $(MAKE) $(MFLAGS)
biblioteca : force_look
	$(ECHO) looking into biblioteca : $(MAKE) $(MFLAGS)
	cd biblioteca; $(MAKE) $(MFLAGS)
client : force_look
	$(ECHO) looking into client : $(MAKE) $(MFLAGS)
	cd client; $(MAKE) $(MFLAGS)
server : force_look
	$(ECHO) looking into server : $(MAKE) $(MFLAGS)
	cd server; $(MAKE) $(MFLAGS)
stub : 	force_look
	$(ECHO) making stub/skeletons : $(BIN_DIR)$(STUB_FILE)
	$(ECHO) $(RMIC) $(STUB_FILE)
	cd ../bin/; $(RMIC) $(STUB_FILE)	
estat : force_look
	$(ECHO) making estat dir : $(MAKE_DIR) $(ESTAT)
	cd $(BIN_DIR); $(MAKE_DIR) $(ESTAT)
clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(BIN) $(OBJS) $(OBJLIBS) *~ estat/* output
	$(ECHO) -$(RM) -rf $(BIN_DIR)*
	-$(RM) -rf $(BIN_DIR)*
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
