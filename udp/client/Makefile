include ../Makefile.inc
CFLAGS	= $(PRJCFLAGS) -I..
OBJLIBS	=  ../libclient.a
OBJS	= udp_client.o client.o

all : $(OBJLIBS)

../libclient.a : $(OBJS)
	$(ECHO) $(AR) $(ARFLAGS) rv ../libclient.a $?
	$(AR) $(ARFLAGS) rv ../libclient.a $?
	$(ECHO) $(RANLIB) ../libclient.a
	$(RANLIB) ../libclient.a

clean :
	$(ECHO) cleaning up in client
	-$(RM) -f $(OBJS) *~

force_look :
	true
