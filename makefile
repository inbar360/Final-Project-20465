FLAGS = -ansi -Wall -pedantic
GLOBAL_DEPS = globals.h
EXE_DEPS = assembler.o utils.o table.o macro.o

assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	gcc -g $(EXE_DEPS) $(FLAGS) -o $@

assembler.o: assembler.c globals.h utils.h table.h macro.h
	gcc -c assembler.c $(FLAGS) -o $@

utils.o: utils.c utils.h globals.h
	gcc -c utils.c $(FLAGS) -o $@

table.o: table.c table.h globals.h
	gcc -c table.c $(FLAGS) -o $@

macro.o: macro.c macro.h globals.h utils.h table.h
	gcc -c macro.c $(FLAGS) -o $@

clean:
	rm *.o