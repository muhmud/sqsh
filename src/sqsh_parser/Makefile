CC       = gcc
CFLAGS   = -c -g -O2 
CPPFLAGS = 
LEX      = flex -Cm
LEMON    = lemon -q

libsqsh: sqsh_parser.o tsql.o tsql.yy.o
	ar rcs sqsh_parser.a sqsh_parser.o tsql.o tsql.yy.o

sqsh_parser.o: sqsh_parser.c sqsh_parser.h
	${CC} $(CFLAGS) -o $@ sqsh_parser.c

tsql.o: tsql.c sqsh_parser.h
	${CC} $(CFLAGS) -o $@ tsql.c

tsql.yy.o: tsql.yy.c tsql.h
	${CC} $(CFLAGS) -o $@ tsql.yy.c

tsql.c: tsql.y
	${LEMON} -s -q tsql.y

tsql.yy.c: tsql.l
	${LEX} -o tsql.yy.c tsql.l

clean:
	rm -f sqsh_parser.o tsql.o tsql.yy.o sqsh_parser.a

distclean:
	rm -f Makefile sqsh_parser.o tsql.o tsql.yy.o tsql.c tsql.yy.c tsql.h sqsh_parser.a
