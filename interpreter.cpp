#include "interpreter.hpp"

VariableStateManager::VariableStateManager()
{
    pushState();
}

void VariableStateManager::pushState()
{
    std::map<std::string, int> newState;
    states.push_back(newState);
}

void VariableStateManager::popState()
{
    states.pop_back();
}

void VariableStateManager::setVariable(std::string key, int value)
{
    states[states.size()-1][key] = value;
}

bool VariableStateManager::hasVariable(std::string key)
{
    return states[states.size()-1].count(key) > 0;
}

int VariableStateManager::getVariable(std::string key)
{
    return states[states.size()-1][key];
}

Interpreter* Interpreter::fromSource(std::string source)
{
    TreeGenerator *tg = TreeGenerator::fromString(source);
    std::vector<TreeNode*> baseNodes = tg->buildTree();
    delete tg;
    return new Interpreter(baseNodes);
}

int Interpreter::evaluateFunction(const TreeNode *functionNode, std::vector<int> args)
{

}

int Interpreter::conditional(const IfNode *node)
{
    int result = evaluate(node->condition);
    if (result != 0)
        return evaluate(node->body);
    return result;
}

int Interpreter::evalBlock(const BlockNode *node)
{
    int result = 0;
    for (TreeNode *n : node->body)
    {
        result = evaluate(n);
    }
    return result;
}

int Interpreter::funcCall(const FunctionCallNode *node)
{
    if (node->functionName == "Out")
    {
        for (int i = 0; i < node->functionArguments.size(); ++i)
        {
            int result = evaluate(node->functionArguments[i]);
            if (i > 0)
                std::cout << ", ";
            std::cout << result;
        }
        std::cout << std::endl;
        return 1;
    }
    else
    {
        throw new std::exception();
    }
}

int Interpreter::resolve(const ValueNode *node)
{
    if (!node->isConstant)
    {
        if (variableManager.hasVariable(node->value))
            return variableManager.getVariable(node->value);
        else
            return 0;
    }
    else
    {
        return std::stoi(node->value);
    }
}

int Interpreter::performOperator(const OperatorNode *node)
{
    int left = evaluate(node->lhs);
    int right = evaluate(node->rhs);
    if (node->op == "==")
        return left == right ? 1 : 0;
    else if (node->op == ">=")
        return left >= right ? 1 : 0;
    else if (node->op == "<=")
        return left <= right ? 1 : 0;
    else if (node->op == "!=")
        return left != right ? 1 : 0;
    else if (node->op == ">")
        return left > right ? 1 : 0;
    else if (node->op == "<")
        return left < right ? 1 : 0;
    else if (node->op == "-")
        return left - right;
    else if (node->op == "+")
        return left + right;
    else if (node->op == "*")
        return left*right;
    else if (node->op == "/")
        return left/right;
    else if (node->op == "=")
    {
        if (node->lhs->type != TN_VALUE || dynamic_cast<const ValueNode*>(node->lhs)->isConstant)
            throw new std::exception();
        std::string varName = dynamic_cast<const ValueNode*>(node->lhs)->value;
        variableManager.setVariable(varName, right);
        return right;
    }
}

int Interpreter::evaluate(const TreeNode *node)
{
    if (node->type == TN_VALUE)
        return resolve(dynamic_cast<const ValueNode*>(node));
    else if (node->type == TN_GROUP)
        return evaluate(dynamic_cast<const GroupNode*>(node)->expression);
    else if (node->type == TN_IF)
        return conditional(dynamic_cast<const IfNode*>(node));
    else if (node->type == TN_BLOCK)
        return evalBlock(dynamic_cast<const BlockNode*>(node));
    else if (node->type == TN_FUNC_CALL)
        return funcCall(dynamic_cast<const FunctionCallNode*>(node));
    else if (node->type == TN_OPERATOR)
        return performOperator(dynamic_cast<const OperatorNode*>(node));
}

Interpreter::Interpreter(std::vector<TreeNode*> baseNodes) : baseNodes(baseNodes)
{

}

Interpreter::~Interpreter()
{
    for (TreeNode *bn : baseNodes)
        delete bn;
}

int Interpreter::start()
{
    int result;
    for (TreeNode* baseNode : baseNodes)
    {
        result = evaluate(baseNode);
    }
    return result;
}
