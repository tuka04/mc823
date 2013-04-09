include Makefile.inc

DIRS	= common client server
BIN	= main
OBJS	= main.o common/common.o common/error.o common/archives.o
OBJLIBS	= libcommon.a libserver.a libclient.a
LIBS	= -L. -lcommon -lserver -lclient

all : $(BIN)

$(BIN) : main.o $(OBJLIBS)
	$(ECHO) $(LD) -o $(BIN) $(OBJS) $(LIBS)  $<
	$(LD) -o $(BIN) $(OBJS) $(LIBS)


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
	-$(RM) -f $(BIN) $(OBJS) $(OBJLIBS) *~
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

force_look :
	true
