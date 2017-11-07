g++ -c lexer.cpp -static -std=c++11
g++ -c treegen.cpp -static -std=c++11
g++ -c main.cpp -static -std=c++11
g++ -o lang.out lexer.o treegen.o main.o -static -std=c++11