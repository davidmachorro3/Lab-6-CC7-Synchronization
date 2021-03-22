all: 
	gcc -pthread createThread.c -o createThread.o
	gcc -pthread joinThread.c -o joinThread.o
	gcc -pthread mutexThread.c -o mutexThread.o
	gcc -pthread conditionThread.c -o conditionThread.o

Alquimia:
	gcc -pthread list.c alquimia.c -o alquimia.o && ./alquimia.o

clean: 
	rm -f *.o