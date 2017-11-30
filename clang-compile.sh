cd build
clang++ -g -c ../src/lexer.cpp -std=c++11
clang++ -g -c ../src/treegen.cpp -std=c++11
clang++ -g -c ../src/main.cpp -std=c++11
clang++ -g -c ../src/interpreter.cpp -std=c++11
clang++ -g -c ../src/standard_functions.cpp -std=c++11
clang++ -g -o xhaust.exe lexer.o treegen.o interpreter.o standard_functions.o main.o -std=c++11
cd ..