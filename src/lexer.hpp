#ifndef Lexer_hpp
#define Lexer_hpp

#include <vector>
#include <string>
#include <stdexcept>

#define TK_IDENTIFIER 1
#define TK_BINARYOPERATOR 2
#define TK_REGULATOR 3
#define TK_KEYWORD 4
#define TK_NUMBER 5
#define TK_STRING 6

class SyntaxException : public std::exception
{
  public:
    int line;
    std::string msg;

    SyntaxException(int line, std::string msg) : line(line), msg(msg) {}
    virtual ~SyntaxException() throw() = default;

    virtual const char *what() const throw()
    {
        return msg.c_str();
    }
};

struct Token
{
    std::string value;
    int type;
};

std::vector<Token> tokenize(std::string source);

#endif
