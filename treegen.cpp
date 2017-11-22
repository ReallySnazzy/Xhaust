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
    if (token.type != TK_IDENTIFIER && token.type != TK_NUMBER)
    {
        if (token.type == TK_OPERATOR && token.value == "(")
            return parseGroup();
        else if (token.type == TK_KEYWORD && token.value == "do")
            return parseFunctionCall();
        else
            throw new SyntaxException(-1, "Expected number or identifier");
    }
    marker++;
    return new ValueNode(token.value, token.type == TK_NUMBER);
}

TreeNode* TreeGenerator::parseMultiplication()
{
    TreeNode *lhs = parseFactor();
    if (tokens[marker].type == TK_OPERATOR && (tokens[marker].value == "*" || tokens[marker].value == "/"))
    {
        std::string op = tokens[marker++].value;
        TreeNode *rhs = parseFactor();
        return new OperatorNode(op, lhs, rhs);
    }
    return lhs;
}

TreeNode* TreeGenerator::parseAddition()
{
    TreeNode *lhs = parseMultiplication();
    if (tokens[marker].type == TK_OPERATOR && (tokens[marker].value == "+" || tokens[marker].value == "-"))
    {
        std::string op = tokens[marker++].value;
        TreeNode *rhs = parseMultiplication();
        return new OperatorNode(op, lhs, rhs);
    }
    return lhs;
}

TreeNode* TreeGenerator::parseExpression()
{
    parseAddition();
}

std::vector<TreeNode*> TreeGenerator::buildTree()
{
    std::vector<TreeNode*> nodes;
    while (marker < tokens.size())
    {
        nodes.push_back(parseExpression());
    }
    return nodes;
}
