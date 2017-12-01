CC = clang++

.PHONY: all clean executable

all: executable

interpreter.o:
	$(CC) -c src/interpreter.cpp

lexer.o:
	$(CC) -c src/lexer.cpp

main.o:
	$(CC) -c src/main.cpp

standard_functions.o:
	$(CC) -c src/standard_functions.cpp

treegen.o:
	$(CC) -c src/treegen.cpp

executable: interpreter.o lexer.o main.o standard_functions.o treegen.o
	$(CC) lexer.o main.o standard_functions.o interpreter.o treegen.o

clean:
	rm *.o
