g++ -c lexer.cpp -std=c++11
g++ -c treegen.cpp -std=c++11
g++ -c main.cpp -std=c++11
g++ -o lang.exe lexer.o treegen.o main.o -static -std=c++11
