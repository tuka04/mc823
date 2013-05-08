include ../Makefile.inc
CFLAGS	= $(PRJCFLAGS) -I..
OBJLIBS	=  ../libcommon.a 
OBJS	= common.o error.o log.o books.o archives.o tcp.o avl.o

all : $(OBJLIBS)

../libcommon.a : $(OBJS)
	$(ECHO) $(AR) $(ARFLAGS) rv ../libcommon.a $?
	$(AR) $(ARFLAGS) rv ../libcommon.a $?
	$(AR) $(ARFLAGS) rv ../../udp/libcommon.a $?	
	$(ECHO) $(RANLIB) ../libcommon.a
	$(RANLIB) ../libcommon.a
	$(RANLIB) ../../udp/libcommon.a

clean :
	$(ECHO) cleaning up in common
	-$(RM) -f $(OBJS) *~

force_look :
	true
