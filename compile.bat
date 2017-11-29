g++ -g -c lexer.cpp -std=c++11
g++ -g -c treegen.cpp -std=c++11
g++ -g -c main.cpp -std=c++11
g++ -g -c interpreter.cpp -std=c++11
g++ -g -c standard_functions.cpp -std=c++11
g++ -g -o lang.exe lexer.o treegen.o interpreter.o standard_functions.o main.o -std=c++11

docker run --rm -it -v ~/Projects/Cpp/Xhaust/:/src -w /src d6ed29ffda6b
docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp gcc:4.9 gcc -o myapp myapp.c