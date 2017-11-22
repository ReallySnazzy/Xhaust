#ifndef Treegen_hpp
#define Treegen_hpp

#include "lexer.hpp"

#include<vector>
#include<string>

#define TN_UNDEFINED -1
#define TN_FUNC_CALL 0
#define TN_GROUP 1
#define TN_VALUE 2
#define TN_OPERATOR 3

class TreeNode
{
public:
    int type = TN_UNDEFINED;
    TreeNode() = default;
    virtual ~TreeNode() = default;
    //virtual void print() = 0;
};

class FunctionCallNode : public TreeNode
{
public:
    const std::string functionName;
    const std::vector<TreeNode*> functionArguments;

    FunctionCallNode(std::string functionName, std::vector<TreeNode*> args)
    : functionName(functionName), functionArguments(args) {type = TN_FUNC_CALL;}
    ~FunctionCallNode() {for (TreeNode* tn : functionArguments) if (tn != nullptr) delete tn;}
};

class GroupNode : public TreeNode
{
public:
    const TreeNode *expression = nullptr;
    GroupNode(TreeNode *expr) : expression(expr) {type = TN_GROUP;}
    ~GroupNode() { if (expression != nullptr) delete expression;}
};

class ValueNode : public TreeNode
{
public:
    const std::string value;
    const bool isConstant;
    ValueNode(std::string value, bool isConstant) : value(value), isConstant(isConstant) {type = TN_VALUE;}
};

class OperatorNode : public TreeNode
{
public:
    const std::string op;
    const TreeNode *lhs = nullptr, *rhs = nullptr;

public:
    OperatorNode(std::string op, TreeNode *lhs, TreeNode *rhs) : op(op), lhs(lhs), rhs(rhs) {type = TN_OPERATOR;}
    virtual ~OperatorNode() {if (lhs != nullptr) delete lhs; if (rhs != nullptr) delete rhs;}
};

class TreeGenerator
{
public:
    std::vector<Token> tokens;
    int marker = 0;

    TreeGenerator(std::vector<Token>);
    FunctionCallNode* parseFunctionCall();
    GroupNode* parseGroup();
    TreeNode* parseExpression();
    TreeNode* parseMultiplication();
    TreeNode* parseAddition();
    TreeNode* parseFactor();
    std::vector<TreeNode*> buildTree();
};

#endif
