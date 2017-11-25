g++ -g -c lexer.cpp -std=c++11
g++ -g -c treegen.cpp -std=c++11
g++ -g -c main.cpp -std=c++11
g++ -g -c interpreter.cpp -std=c++11
g++ -g -c standard_functions.cpp -std=c++11
g++ -g -o lang.exe lexer.o treegen.o interpreter.o standard_functions.o main.o -std=c++11
