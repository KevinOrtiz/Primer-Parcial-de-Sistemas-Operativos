# objeto: dependencias (archivos .o)
#	comando
prog: prog.o clientLibrary/clientLibrary.o
	gcc -o prog prog.o clientLibrary/clientLibrary.o

clientLibrary/clientLibrary.o: clientLibrary/clientLibrary.c clientLibrary/clientLibrary.h
	gcc -c clientLibrary/clientLibrary.c

prog.o: prog.c clientLibrary/clientLibrary.c clientLibrary/clientLibrary.h
	gcc -c prog.c

clean:
	rm -f prog *.o
