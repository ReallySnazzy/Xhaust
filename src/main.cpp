#include <iostream>
#include <fstream>
#include "interpreter.hpp"

void runCode(std::string source)
{
    try
    {
        Interpreter *i = Interpreter::fromSource(source);
        std::cout << "Running: " << std::endl << source << std::endl;
        std::cout << "============================================================" << std::endl;
        XhaustValue result = i->start();
        delete i;
        std::cout << "============================================================" << std::endl;
        std::cout << "The program returned " << result.toString() << std::endl;
        std::cout << "============================================================" << std::endl;
    }
    catch (std::exception *ex)
    {
        std::cout << ex->what() << std::endl;
    }
}

void runFile(std::string filename)
{
    std::fstream sourceFile(filename);
    std::string line;
    std::string source;

    std::ifstream in;
    in.open(filename);

    if (!in)
    {
        std::cout << "Error: File not found. It might be inaccessible. Check privileges." << std::endl;
        exit(1);
    }

    while (sourceFile && std::getline(sourceFile, line))
    {
        source += line + "\n";
    }
    runCode(source);
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        std::string fileName = std::string(argv[1]);
        runFile(fileName);
    }
    else
    {
        std::cout << "Usage: xhaust filename.xh" << std::endl;
    }
}
