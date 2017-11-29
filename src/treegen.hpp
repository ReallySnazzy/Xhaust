#ifndef Treegen_hpp
#define Treegen_hpp

#include "lexer.hpp"

#include<vector>
#include<string>
#include<iostream>

#define TN_UNDEFINED -1
#define TN_FUNC_CALL 0
#define TN_GROUP 1
#define TN_VALUE 2
#define TN_OPERATOR 3
#define TN_BLOCK 4
#define TN_IF 5

class TreeNode
{
public:
    int type = TN_UNDEFINED;
    TreeNode() = default;
    virtual ~TreeNode() = default;
    virtual void print(int level) const = 0;
};

class FunctionCallNode : public TreeNode
{
public:
    const std::string functionName;
    const std::vector<TreeNode*> functionArguments;

    FunctionCallNode(std::string functionName, std::vector<TreeNode*> args)
    : functionName(functionName), functionArguments(args) {type = TN_FUNC_CALL;}
    virtual ~FunctionCallNode() {for (TreeNode* tn : functionArguments) if (tn != nullptr) delete tn;}

    virtual void print(int level) const
    {
        for (int i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << "FunctionCall : " << functionName << std::endl;
        for (TreeNode* node : functionArguments)
            node->print(level+1);
    }
};

class GroupNode : public TreeNode
{
public:
    const TreeNode *expression = nullptr;
    GroupNode(TreeNode *expr) : expression(expr) {type = TN_GROUP;}
    virtual ~GroupNode() { if (expression != nullptr) delete expression; expression = nullptr; }

    virtual void print(int level) const
    {
        for (int i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << "Group" << std::endl;
        expression->print(level + 1);
    }
};

class ValueNode : public TreeNode
{
public:
    const std::string value;
    const bool isConstant;
    ValueNode(std::string value, bool isConstant) : value(value), isConstant(isConstant) {type = TN_VALUE;}
    virtual ~ValueNode() = default;

    virtual void print(int level) const
    {
        for (int i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << "Value : " << value << std::endl;
    }
};

class BlockNode : public TreeNode
{
public:
    const std::vector<TreeNode*> body;

    BlockNode(std::vector<TreeNode*> body) : body(body)
    {
        type = TN_BLOCK;
    }

    ~BlockNode()
    {
        for (TreeNode *elem : body)
            delete elem;
    }

    virtual void print(int level) const
    {
        for (int i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << "Block" << std::endl;
        for (TreeNode *elem : body)
            elem->print(level + 1);
    }
};

class IfNode : public TreeNode
{
public:
    const TreeNode* condition;
    const TreeNode* body;

    IfNode(TreeNode *condition, TreeNode *body) : condition(condition), body(body)
    {
        type = TN_IF;
    }

    virtual ~IfNode()
    {
        delete condition;
        delete body;
    }

    virtual void print(int level) const
    {
        for (int i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << "If" << std::endl;
        condition->print(level+1);
        body->print(level+1);
    }
};

class OperatorNode : public TreeNode
{
public:
    const std::string op;
    const TreeNode *lhs = nullptr, *rhs = nullptr;

public:
    OperatorNode(std::string op, TreeNode *lhs, TreeNode *rhs) : op(op), lhs(lhs), rhs(rhs)
    {
        type = TN_OPERATOR;
    }

    virtual ~OperatorNode() {
        if (lhs != nullptr) delete lhs;
        if (rhs != nullptr) delete rhs;
    }

    virtual void print(int level) const
    {
        for (int i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << "Operator : " << op << std::endl;
        lhs->print(level + 1);
        rhs->print(level + 1);
    }
};

class TreeGenerator
{
public:
    std::vector<Token> tokens;
    int marker = 0;

    TreeGenerator(std::vector<Token>);
    static TreeGenerator* fromString(std::string source);
    FunctionCallNode* parseFunctionCall();
    GroupNode* parseGroup();
    TreeNode* parseExpression();
    TreeNode* parseMultiplication();
    TreeNode* parseAddition();
    TreeNode* parseAssignment();
    TreeNode* parseFactor();
    BlockNode* parseBlock();
    TreeNode* parseConditional();
    IfNode* parseIf();
    TreeNode* parseStatement();
    std::vector<TreeNode*> buildTree();
};

#endif
