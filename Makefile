CC = gcc
RM = rm -f
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
