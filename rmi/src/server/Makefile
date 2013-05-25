include ../Makefile.inc

COMP = comp
OBJS = *.java
OBJS_BIN = *.class
OBJ_RUN = Server


all: $(COMP)

comp: 
	$(ECHO) $(LD) $(PRJCFLAGS) $(PACKAGE) $(LDFLAGS) $(PACKAGE) $(OBJS)
	$(LD) $(PRJCFLAGS) $(PACKAGE) $(LDFLAGS) $(PACKAGE) $(OBJS)
clean :
	$(ECHO) cleaning up in server
	-$(RM) -f $(OBJS_BIN) *~

force_look :
	true
