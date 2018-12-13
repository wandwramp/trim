CC = gcc
RM = rm -f
ifndef INSTALLDIR
INSTALLDIR=~/wramp-install/
endif
MKDIR=mkdir -p
COPY=cp
BUILDBINS=trim 
INSTALLBINS=$(INSTALLDIR)trim
CFLAGS = -Wall -O3

.c.o:	$<
	$(CC) $(CFLAGS) -c $<

all:	trim

trim:	trim.c
	$(CC) $(CFLAGS) -o trim trim.c

clean:
	$(RM) *.o *~
	@echo Type \"make clobber\" to clean binaries.

clobber:
	$(RM) trim
	$(RM) $(INSTALLBINS)
	
install: all
	$(MKDIR) $(INSTALLDIR)
	$(COPY) $(BUILDBINS) $(INSTALLDIR)
