CC=gcc
CFLAGS=-g
TARGET: test.exe

OBJS=glthread/glthread.o \
							graph.o 	 \
							topologies.o \
							net.o		 \
							utils.o

test.exe:testapp.o ${OBJS}
		${CC} ${CFLAGS} testapp.o ${OBJS} -o test.exe

testapp.o: testapp.c
		${CC} ${CFLAGS} -c testapp.c -o testapp.o

glthread/glthread.o: glthread/glthread.c
		${CC} ${CFLAGS} -c -I glthread glthread/glthread.c -o glthread/glthread.o

graph.o:graph.c
		${CC} ${CFLAGS} -c -I . graph.c -o graph.o

topologies.o:topologies.c
		${CC} ${CFLAGS} -c -I . topologies.c -o topologies.o

net.o:net.c
		${CC} ${CFLAGS} -c -I . net.c -o net.o

utils.o:utils.c	
		${CC} ${CFLAGS} -c -I . utils.c -o utils.o
clean:
	rm *.o
	rm glthread/glthread.o
	rm *exe
