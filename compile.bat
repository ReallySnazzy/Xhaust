echo # Bro check me out this batch script is also a bash script because I am amazing > NUL
echo # Although, tbh the hardest part was these comments > NUL

cd build
g++ -g -c ../src/lexer.cpp -std=c++11
g++ -g -c ../src/treegen.cpp -std=c++11
g++ -g -c ../src/main.cpp -std=c++11
g++ -g -c ../src/interpreter.cpp -std=c++11
g++ -g -c ../src/standard_functions.cpp -std=c++11
g++ -g -o xhaust.exe lexer.o treegen.o interpreter.o standard_functions.o main.o -std=c++11
cd ..
exit $?