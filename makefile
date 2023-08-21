FLAGS = -ansi -Wall -pedantic
GLOBAL_DEPS = globals.h
EXE_DEPS = assembler.o utils.o macro.o preprocessor.o tables.o pass_functions.o output_files.o first_pass.o sec_pass.o 

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

tables.o: tables.c tables.h globals.h
	gcc -c tables.c $(FLAGS) -o $@

pass_functions.o: pass_functions.c pass_functions.h globals.h tables.h utils.h
	gcc -c pass_functions.c $(FLAGS) -o $@

output_files.o: output_files.c output_files.h globals.h tables.h utils.h
	gcc -c output_files.c $(FLAGS) -o $@

first_pass.o: first_pass.c first_pass.h globals.h pass_functions.h
	gcc -c first_pass.c $(FLAGS) -o $@

sec_pass.o: sec_pass.c sec_pass.h globals.h tables.h utils.h pass_functions.h
	gcc -c sec_pass.c $(FLAGS) -o $@

clean:
	rm *.o