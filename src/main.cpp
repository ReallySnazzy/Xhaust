
#include <iostream>
#include <fstream>
#include "interpreter.hpp"

bool DEBUG_PRINT_TOKENS = false;
bool DEBUG_PRINT_TREE = false; //doesn't do anything atm

//This file should be renamed to Xhaust.cpp for coolness

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
XhaustValue runCode(Interpreter *i)
{
    return i->start();
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

    if (DEBUG_PRINT_TOKENS == true)
    {
        std::cout << "Tokens:" << std::endl;
        std::vector<Token> tokens = tokenize(source);
        for (Token t : tokens)
        {
            std::cout << t.value << " [" << t.type << "]" << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Running: " << getFilename(filepath) << std::endl;
    std::cout << "Interpreting..." << std::endl;
    Interpreter *i;
    try
    {
        i = Interpreter::fromSource(source);
    }
    catch (std::exception *ex)
    {
        std::cout << "Interpretation error." << std::endl;
        std::cout << ex->what() << std::endl;
        std::exit(1);
    }
    std::cout << "=============================BEGIN=============================" << std::endl;
    try
    {
        XhaustValue result = runCode(i);
        delete i;
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
    //todo add some more debug modes
    //todo support real arg flags
    if (argc > 2 && strncmp(argv[1], "--print-tokens", 14) == 0)
    {
        DEBUG_PRINT_TOKENS = true;
        std::string fileName = std::string(argv[2]);
        runFile(fileName);
    }
    if (argc > 2 && strncmp(argv[1], "--print-parseTree", 17) == 0)
    {
        DEBUG_PRINT_TREE = true;
        std::string fileName = std::string(argv[2]);
        runFile(fileName);
    }
    else if (argc > 1)
    {
        std::string fileName = std::string(argv[1]);
        runFile(fileName);
    }
    else
    {
        std::cout << "Usage: xhaust filename.xh" << std::endl;
    }
}
