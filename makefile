assembler: assembler.o macro.o macro.h
	gcc -Wall -ansi -pedantic assembler.o macro.o -o assembler
assembler.o: assembler.c macro.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
macro.o: macro.c macro.h
	gcc -c -Wall -ansi -pedantic macro.c -o macro.o
clean:
	rm *.o
