#include "treegen.hpp"

TreeGenerator* TreeGenerator::fromString(std::string source)
{
    return new TreeGenerator(tokenize(source));
}

TreeGenerator::TreeGenerator(std::vector<Token> tokens)
{
    this->tokens = tokens;
}

FunctionCallNode* TreeGenerator::parseFunctionCall()
{
    if (tokens[marker++].value != "do")
    {
        throw new SyntaxException(-1, "Expected do at beginning of function call");
    }
    std::string functionIdentifier = tokens[marker++].value;
    if (tokens[marker++].value != "(")
    {
        throw new SyntaxException(-1, "Expected ( at the beginning of function arguments");
    }
    std::vector<TreeNode*> arguments;
    if (tokens[marker].value != ")")
    {
        do {
            arguments.push_back(parseExpression());
        } while (tokens[marker++].value == ",");
        --marker;
    }
    if (tokens[marker++].value != ")")
    {
        throw new SyntaxException(-1, "Expected ) at the end of function arguments");
    }
    return new FunctionCallNode(functionIdentifier, arguments);
}

GroupNode* TreeGenerator::parseGroup()
{
    ++marker;
    GroupNode *res = new GroupNode(parseExpression());
    if (tokens[marker++].value != ")") {
        throw new SyntaxException(-1, ") expected to close expression");
    }
    return res;
}

TreeNode* TreeGenerator::parseFactor()
{
    Token token = tokens[marker];
    if (token.type != TK_IDENTIFIER && token.type != TK_NUMBER && token.type != TK_STRING)
    {
        if (token.type == TK_OPERATOR && token.value == "(")
            return parseGroup();
        else if (token.type == TK_KEYWORD && token.value == "do")
            return parseFunctionCall();
        else
            throw new SyntaxException(-1, "Expected number or identifier, got " + token.value);
    }
    marker++;
    return new ValueNode(token.value, token.type == TK_NUMBER || token.type == TK_STRING);
}

TreeNode* TreeGenerator::parseMultiplication()
{
    TreeNode *lhs = parseFactor();
    if (tokens.size() > marker && tokens[marker].type == TK_OPERATOR && (tokens[marker].value == "*" || tokens[marker].value == "/"))
    {
        std::string op = tokens[marker++].value;
        TreeNode *rhs = parseMultiplication();
        return new OperatorNode(op, lhs, rhs);
    }
    return lhs;
}

TreeNode* TreeGenerator::parseAddition()
{
    TreeNode *lhs = parseMultiplication();
    if (tokens.size() > marker && tokens[marker].type == TK_OPERATOR && (tokens[marker].value == "+" || tokens[marker].value == "-"))
    {
        std::string op = tokens[marker++].value;
        TreeNode *rhs = parseAddition();
        return new OperatorNode(op, lhs, rhs);
    }
    return lhs;
}

TreeNode* TreeGenerator::parseConditional()
{
    TreeNode *lhs = parseAddition();
    if (tokens.size() > marker + 1 && tokens[marker].type == TK_OPERATOR)
    {
        if (tokens[marker].value == "==" || tokens[marker].value == ">=" || tokens[marker].value == "<="
            || tokens[marker].value == "!=" || tokens[marker].value == ">" || tokens[marker].value == "<")
        {
            std::string conditionalOp = tokens[marker++].value;
            TreeNode *rhs = parseAddition();
            return new OperatorNode(conditionalOp, lhs, rhs);
        }
    }
    return lhs;
}

TreeNode* TreeGenerator::parseAssignment()
{
    if (tokens.size() > marker + 1 && tokens[marker].type == TK_IDENTIFIER && tokens[marker+1].value == "=")
    {
        TreeNode *lhs = new ValueNode(tokens[marker++].value, false);
        marker++; // Skip over =
        TreeNode *rhs = parseConditional();
        return new OperatorNode("=", lhs, rhs);
    }
    else
    {
        return parseConditional();
    }
}

BlockNode* TreeGenerator::parseBlock()
{
    if (tokens.size() > marker && tokens[marker].type == TK_OPERATOR && tokens[marker].value == "{")
        marker++;
    else
        throw new SyntaxException(-1, "{ expected.");
    std::vector<TreeNode*> body;
    while (tokens.size() > marker && (tokens[marker].type != TK_OPERATOR || tokens[marker].value != "}"))
    {
        body.push_back(parseStatement());
    }
    if (tokens.size() > marker && tokens[marker].type == TK_OPERATOR && tokens[marker].value == "}")
        ++marker;
    else
        throw new SyntaxException(-1, "} expected.");
    return new BlockNode(body);
}

IfNode* TreeGenerator::parseIf()
{
    if (tokens[marker].type == TK_KEYWORD && tokens[marker].value == "if")
        ++marker;
    else
        throw new SyntaxException(-1, "if expected");
    if (tokens.size() > marker && tokens[marker].type == TK_OPERATOR && tokens[marker].value == "(")
        ++marker;
    else
        throw new SyntaxException(-1, "( expected after if");
    TreeNode *condition = parseExpression();
    if (tokens.size() > marker && tokens[marker].type == TK_OPERATOR && tokens[marker].value == ")")
        ++marker;
    else
        throw new SyntaxException(-1, ") expected after if condition");
    TreeNode *block = parseBlock();
    return new IfNode(condition, block);
}

TreeNode* TreeGenerator::parseStatement()
{
    if (tokens.size() > marker && tokens[marker].type == TK_KEYWORD)
    {
        if (tokens[marker].value == "if")
            return parseIf();
        else if (tokens[marker].value == "do")
            return parseFunctionCall();
        else
            throw new SyntaxException(-1, "Unknown keyword: " + tokens[marker].value);
    }
    else
    {
        return parseExpression();
    }
}

TreeNode* TreeGenerator::parseExpression()
{
    return parseAssignment();
}

std::vector<TreeNode*> TreeGenerator::buildTree()
{
    std::vector<TreeNode*> nodes;
    while (marker < tokens.size())
    {
        int old = marker;
        nodes.push_back(parseStatement());
        if (old == marker)
            throw new SyntaxException(-1, "wtf");
    }
    return nodes;
}
