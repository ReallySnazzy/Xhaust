#include "lexer.hpp"
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

class Lexer
{
  public:
    std::string source;
    std::vector<Token> tokens;
    int marker = 0;

    Lexer(std::string source)
    {
        this->source = source;
    }

    const std::vector<std::string> OPERATORS = {">=", "<=", "<", "%", ">", "==", "=", "!=", "+", "-", "**", "*", "/", "^", "~", "!", "(", ")", "[", "]", "{", "}", ";", ","};
    const std::vector<std::string> KEYWORDS = {"exhaust", "do", "if"};

    void addToken(std::string value, int type)
    {
        Token t;
        t.value = value;
        t.type = type;
        tokens.push_back(t);
    }

    void readName(int &marker)
    {
        std::string word;
        while (isLetter(source[marker]))
        {
            word += source[marker];
            marker++;
        }
        bool isKeyword = (std::find(KEYWORDS.begin(), KEYWORDS.end(), word) != KEYWORDS.end());
        addToken(word, isKeyword ? TK_KEYWORD : TK_IDENTIFIER);
    }

    void readNum(int &marker)
    {
        std::string num;
        while (isNumber(source[marker]))
        {
            num += source[marker++];
        }
        addToken(num, TK_NUMBER);
    }

    void skipWhitespace(int &marker)
    {
        while (isWhitespace(source[marker++]))
            ;
        --marker;
    }

    void skipComment(int &marker)
    {
        while (marker < source.length() && source[marker] != '\n' && source[marker] != '\r')
            marker++;
    }

    void readString(int &marker)
    {
        std::string str = "";
        str += source[marker++];
        while (marker < source.length())
        {
            str += source[marker];

            //TODO unescape special characters like \\, \n, \"
            if (source[marker] == '"' && source[marker - 1] != '\\')
            {
                break;
            }
            marker++;
        }
        marker++;
        addToken(str, TK_STRING);
    }

    void readOperator(int &marker)
    {
        bool added = false;
        for (std::string possibleOperator : OPERATORS)
        {
            if (source.substr(marker, possibleOperator.length()) == possibleOperator)
            {
                marker += possibleOperator.length();
                addToken(possibleOperator, TK_OPERATOR);
                added = true;
                break;
            }
        }
        if (!added)
        {
            int code = source[marker];
            throw new SyntaxException(-1, std::string("Unknown symbol ") + source[marker] + "(" + std::to_string(code) + ")");
        }
    }

    bool isLetter(char c)
    {
        return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_' || c == '$';
    }

    bool isNumber(char c)
    {
        return ('0' <= c && c <= '9') || c == '.';
    }

    bool isWhitespace(char c)
    {
        return c == ' ' || c == '\n' || c == '\r' || c == '\t';
    }

    void read()
    {
        for (int len = source.length(); marker < len;)
        {
            char c = source[marker];

            if (c == '"')
            {
                readString(marker);
            }
            else if (isLetter(c))
            {
                readName(marker);
            }
            else if (isNumber(c))
            {
                readNum(marker);
            }
            else if (isWhitespace(c))
            {
                skipWhitespace(marker);
            }
            else if (c == '/' && marker + 1 < len && source[marker + 1] == '/')
            {
                skipComment(marker);
            }
            else
            {
                readOperator(marker);
            }
        }
    }

    std::vector<Token> getTokens()
    {
        return tokens;
    }

    ~Lexer() = default;
};

std::vector<Token> tokenize(std::string source)
{
    Lexer l(source);
    l.read();
    return l.getTokens();
}
