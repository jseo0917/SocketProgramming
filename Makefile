CC=gcc
CFLAGS=-g
TARGET: test.exe
LIBS=-lpthread -L ./CommandParser -lcli
OBJS=glthread/glthread.o \
							graph.o 	 \
							topologies.o \
							net.o		 \
							Layer2/layer2.o \
							nwcli.o  	\
							utils.o		 \
							communication.o \
									
test.exe:testapp.o ${OBJS} CommandParser/libcli.a
		${CC} ${CFLAGS} testapp.o ${OBJS} -o test.exe ${LIBS}

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

communication.o:communication.c
		${CC} ${CFLAGS} -c -I . communication.c -o communication.o

Layer2/layer2.o: Layer2/layer2.c
		${CC} ${CFLAGS} -c -I . Layer2/layer2.c -o Layer2/layer2.o

nwcli.o:nwcli.c
		${CC} ${CFLAGS} -c -I . nwcli.c -o nwcli.o
		
CommandParser/libcli.a:
	(cd CommandParser; make)

clean:
	rm *.o
	rm glthread/glthread.o
	rm Layer2/layer2.o
	rm *exe
	(cd CommandParser; make clean)
all:
	make
	(cd CommandParser; make)