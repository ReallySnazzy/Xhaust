#include <iostream>
#include <fstream>
#include "interpreter.hpp"

#if defined(_WIN32) || defined(_WIN64)
    // Hack to add chdir to windows
    #include <windows.h>
    int chdir(const char *dir)
    {
        if (SetCurrentDirectory(dir))
            return 0;
        return -1;
    }
#else
    #include <unistd.h>
#endif

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

std::string getDirectory(std::string filename)
{
    return filename.substr(0, filename.find_last_of("\\/"));
}

void runFile(std::string filename)
{
    std::fstream sourceFile(filename);
    std::string line;
    std::string source;

    if (!sourceFile)
    {
        std::cout << "Error: File not found. It might be inaccessible. Check privileges." << std::endl;
        exit(1);
    }

    chdir(getDirectory(filename).c_str());

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
