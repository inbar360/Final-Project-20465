FLAGS = -ansi -Wall -pedantic
GLOBAL_DEPS = globals.h
EXE_DEPS = assembler.o utils.o macro.o preprocessor.o

assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	gcc -g $(EXE_DEPS) $(FLAGS) -o $@

assembler.o: assembler.c globals.h utils.h macro.h preprocessor.h
	gcc -c assembler.c $(FLAGS) -o $@

utils.o: utils.c utils.h globals.h
	gcc -c utils.c $(FLAGS) -o $@

macro.o: macro.c macro.h globals.h
	gcc -c macro.c $(FLAGS) -o $@

preprocessor.o: preprocessor.c preprocessor.h globals.h utils.h macro.h
	gcc -c preprocessor.c $(FLAGS) -o $@

clean:
	rm *.o