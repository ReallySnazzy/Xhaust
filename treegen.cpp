#include "treegen.hpp"

TreeGenerator::TreeGenerator(std::vector<Token> tokens) {
    this->tokens = tokens;
    this->marker = marker;
}

FunctionCallNode* TreeGenerator::parseFunctionCall()
{
    if (tokens[marker].value != "do")
    {
        throw new SyntaxException(-1, "Expected do at beginning of function call");
    }
    ++marker;
    std::string functionIdentifier = tokens[marker].value;
    ++marker;
    if (tokens[marker].value != "(") 
    {
        throw new SyntaxException(-1, "Expected ( at the beginning of function arguments");
    }
    ++marker;
    std::vector<TreeNode*> arguments;
    do {
        arguments.push_back(parseExpression());
    } while (tokens[marker++].value == ",");
    if (tokens[marker-1].value != ")")
    {
        throw new SyntaxException(-1, "Expected ) at the end of function arguments");
    }
    return new FunctionCallNode(functionIdentifier, arguments);
}

GroupNode* TreeGenerator::parseGroup()
{
    ++marker;
    GroupNode *res = new GroupNode(parseExpression());
    if (tokens[marker].value != ")") {
        throw new SyntaxException(-1, ") expected to close expression");
    }
}

TreeNode* TreeGenerator::appendExpressionsTogetherOrFinish(TreeNode* lhs) 
{
    if (tokens.size() > marker + 1 && tokens[marker+1].type == TK_OPERATOR
        && tokens[marker+1].value != ")" && tokens[marker+1].value != ",") 
    {
        std::string op = tokens[marker+1].value;
        marker += 2;
        return new OperatorNode(op, lhs, parseExpression());
    } 
    else
    {
        return lhs;
    }
}

TreeNode* TreeGenerator::parseExpression() 
{
    if (tokens[marker].type == TK_KEYWORD && tokens[marker].value == "do") 
    {
        FunctionCallNode *node = parseFunctionCall();
        return appendExpressionsTogetherOrFinish(node);
    } 
    else if (tokens[marker].type == TK_OPERATOR && tokens[marker].value == "(") 
    {
        GroupNode *group = parseGroup();
        return appendExpressionsTogetherOrFinish(group);
    }
    // TODO: Respect pemdas
    else if (tokens[marker].type == TK_IDENTIFIER || tokens[marker].type == TK_NUMBER) 
    {
        ValueNode *val = new ValueNode(tokens[marker].value, tokens[marker].type == TK_IDENTIFIER);
        return appendExpressionsTogetherOrFinish(val);
    }
}

std::vector<TreeNode*> TreeGenerator::buildTree() 
{
    std::vector<TreeNode*> nodes;
    while (marker < tokens.size()) {
        nodes.push_back(parseExpression());
    }
    return nodes;
}