output: glthread.o employee.o
	gcc glthread.o employee.o -o output

employee: employee.c
	gcc employee.c -o employee.o -c

glthread: glthread.c glthread.h
	gcc glthread.c -o glthread.o -c

clean:
	rm *.o output employee glthread
