cd build
g++ -g -c ../src/lexer.cpp -std=c++11
g++ -g -c ../src/treegen.cpp -std=c++11
g++ -g -c ../src/main.cpp -std=c++11
g++ -g -c ../src/interpreter.cpp -std=c++11
g++ -g -c ../src/standard_functions.cpp -std=c++11
g++ -g -o lang.exe lexer.o treegen.o interpreter.o standard_functions.o main.o -std=c++11
cd ..