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

//TODO: runCode takes Interpreter obj and exceptions = "run time exception"
//Exceptions creating Interpreter obj should then be like "compile time exception" eg syntax error
XhaustValue runCode(std::string source)
{
    Interpreter *i = Interpreter::fromSource(source);
    XhaustValue result = i->start();
    delete i;
    return result;
}

std::string getDirectory(std::string filename)
{
    return filename.substr(0, filename.find_last_of("\\/"));
}

std::string getFilename(std::string filename)
{
    return filename.substr(filename.find_last_of("\\/") + 1, filename.length());
}

std::string getContentsFromPath(std::string filename)
{
    std::fstream sourceFile(filename);
    std::string line;
    std::string source;

    if (!sourceFile)
    {
        std::cout << "Error: File not found. It might be inaccessible. Check privileges." << std::endl;
        exit(1);
    }

    while (sourceFile && std::getline(sourceFile, line))
    {
        source += line + "\n";
    }

    return source;
}

void runFile(std::string filepath)
{
    std::string source = getContentsFromPath(filepath);
    chdir(getDirectory(filepath).c_str());

    std::cout << "Running: " << getFilename(filepath) << std::endl;
    std::cout << "=============================BEGIN=============================" << std::endl;
    try
    {
        XhaustValue result = runCode(source);
        std::cout << "============================= END =============================" << std::endl;
        std::cout << "The program returned \"" << result.toString() << "\"" << std::endl
                  << std::endl;
    }
    catch (std::exception *ex)
    {
        std::cout << "=============================ERROR=============================" << std::endl;

        std::cout << ex->what() << std::endl;
    }
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
