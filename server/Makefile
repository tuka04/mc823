include ../Makefile.inc
CFLAGS	= $(PRJCFLAGS) -lcommon -I..
OBJLIBS	=  ../libserver.a
OBJS	= tcp_server.o login.o server.o

all : $(OBJLIBS)

../libserver.a : $(OBJS)
	$(ECHO) $(OBJS)
	$(ECHO) $(AR) $(ARFLAGS) rv ../libserver.a $?
	$(AR) $(ARFLAGS) rv ../libserver.a $?
	$(ECHO) $(RANLIB) ../libserver.a
	$(RANLIB) ../libserver.a

clean :
	$(ECHO) cleaning up in server
	-$(RM) -f $(OBJS) *~

force_look :
	true
