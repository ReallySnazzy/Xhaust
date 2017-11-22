#include <iostream>
#include <fstream>
#include "interpreter.hpp"

void runCode(std::string source)
{
    Interpreter *i = Interpreter::fromSource(source);
    std::cout << "Ran: " << source << std::endl;
    std::cout << "============================================================" << std::endl;
    int result = i->start();
    delete i;
    std::cout << "============================================================" << std::endl;
    std::cout << "The program returned " << result << std::endl;
    std::cout << "============================================================" << std::endl;
}

void runFile(std::string fileName)
{
    std::fstream sourceFile(fileName);
    std::string line;
    std::string source;
    while (sourceFile && std::getline(sourceFile, line))
    {
        source += line + "\n";
    }
    runCode(source);
}

int main()
{
    try
    {
        runFile("test.xh");
    }
    catch (std::exception *ex)
    {
        std::cout << ex->what() << std::endl;
    }
}
