#ifndef interpreter_hpp
#define interpreter_hpp 1

#include "treegen.hpp"
#include <map>

// All variables are stored internally as integers.
class VariableStateManager
{
private:
    std::vector<std::map<std::string, int>> states;

public:
    // Starts with an empty state.
    VariableStateManager();

    // Creates a new empty variable table and stores the previous one for later.
    void pushState();

    // Gets rid of the current state and restores the previous one.
    void popState();

    int getVariable(std::string name);
    bool hasVariable(std::string name);
    void setVariable(std::string name, int value);
};

class Interpreter
{
private:
    std::vector<TreeNode*> baseNodes;
    VariableStateManager variableManager;

    Interpreter(std::vector<TreeNode*> nodes);

    int resolve(const ValueNode*);
    int funcCall(const FunctionCallNode*);
    int conditional(const IfNode*);
    int evalBlock(const BlockNode*);

public:
    virtual ~Interpreter();
    static Interpreter* fromSource(std::string source);
    int evaluate(const TreeNode *node);
    int evaluateFunction(const TreeNode *functionNode, std::vector<int> args);
    int performOperator(const OperatorNode *opNode);
    int start();
};

#endif // interpreter_hpp
