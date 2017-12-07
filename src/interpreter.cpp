#include "interpreter.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

#pragma region XhaustValue
XhaustValueTypes XhaustValue::getType() const
{
    return type;
}

std::string XhaustValue::toString() const
{
    XhaustValueTypes type = getType();
    if (type == XhaustValueTypes::string)
    {
        return valueString;
    }
    else if (type == XhaustValueTypes::number)
    {
        std::string res = std::to_string(valueNumber);

        res.erase(std::min(res.find_last_not_of('0') + 2, res.length() - 1), std::string::npos);
        return res;
    }
    else if (type == XhaustValueTypes::boolean)
    {
        return valueBoolean ? "true" : "false";
    }
    else if (type == XhaustValueTypes::object)
    {
        uintptr_t uptr = reinterpret_cast<uintptr_t>(valueObject);
        int ptr = static_cast<int>(uptr);
        std::stringstream ss;
        ss << std::hex << ptr;
        return ss.str();
    }
    else if (type == XhaustValueTypes::nulltype)
    {
        return "null";
    }

    //return required for compilation in some flavours of OSX :o
    return NULL;
}

XhaustValue XhaustValue::operator==(const XhaustValue &other) const
{
    if (other.getType() == XhaustValueTypes::string || getType() == XhaustValueTypes::string)
    {
        return XhaustValue::fromBoolean(toString() == other.toString());
    }
    else
    {
        return XhaustValue::fromBoolean(getNumberValue() == other.getNumberValue());
    }
}

XhaustValue XhaustValue::operator>=(const XhaustValue &other) const
{
    if (other.getType() == XhaustValueTypes::string || getType() == XhaustValueTypes::string)
    {
        return XhaustValue::fromNull();
    }
    else
    {
        return XhaustValue::fromBoolean(getNumberValue() >= other.getNumberValue());
    }
}

XhaustValue XhaustValue::operator<=(const XhaustValue &other) const
{
    if (other.getType() == XhaustValueTypes::string || getType() == XhaustValueTypes::string)
    {
        return XhaustValue::fromNull();
    }
    else
    {
        return XhaustValue::fromBoolean(other.getNumberValue() <= getNumberValue());
    }
}

XhaustValue XhaustValue::operator>(const XhaustValue &other) const
{
    if (other.getType() == XhaustValueTypes::string || getType() == XhaustValueTypes::string)
    {
        return XhaustValue::fromNull();
    }
    else
    {
        return XhaustValue::fromBoolean(getNumberValue() > other.getNumberValue());
    }
}

XhaustValue XhaustValue::operator<(const XhaustValue &other) const
{
    if (other.getType() == XhaustValueTypes::string || getType() == XhaustValueTypes::string)
    {
        return XhaustValue::fromNull();
    }
    else
    {
        return XhaustValue::fromBoolean(getNumberValue() < other.getNumberValue());
    }
}

XhaustValue XhaustValue::operator+(const XhaustValue &other) const
{
    if (other.getType() == XhaustValueTypes::string || getType() == XhaustValueTypes::string)
    {
        std::string left = toString();
        std::string right = other.toString();
        return XhaustValue::fromString(left + right);
    }
    else if (other.getType() == XhaustValueTypes::number || getType() == XhaustValueTypes::number
        || other.getType() == XhaustValueTypes::object || getType() == XhaustValueTypes::object
        || other.getType() == XhaustValueTypes::nulltype || getType() == XhaustValueTypes::nulltype)
    {
        double left = getNumberValue();
        double right = other.getNumberValue();
        return XhaustValue::fromNumber(left + right);
    }
    else //if (other.getType() == XhaustValueTypes::boolean || getType() == XhaustValueTypes::boolean)
    {
        return XhaustValue::fromBoolean(valueBoolean && other.valueBoolean);
    }
}

XhaustValue XhaustValue::operator-(const XhaustValue &other) const
{
    if (getType() == XhaustValueTypes::string || other.getType() == XhaustValueTypes::string)
    {
        return XhaustValue::fromNumber(valueString.compare(other.valueString));
    }
    else if ((other.getType() == XhaustValueTypes::number || other.getType() == XhaustValueTypes::nulltype) && (getType() == XhaustValueTypes::number || getType() == XhaustValueTypes::nulltype))
    {
        return XhaustValue::fromNumber(getNumberValue() - other.getNumberValue());
    }
    else
    {
        return XhaustValue::fromNull();
    }
}

XhaustValue XhaustValue::operator*(const XhaustValue &other) const
{
    if ((other.getType() == XhaustValueTypes::number || other.getType() == XhaustValueTypes::nulltype) && (getType() == XhaustValueTypes::number || getType() == XhaustValueTypes::nulltype))
    {
        return XhaustValue::fromNumber(getNumberValue() * other.getNumberValue());
    }
    else
    {
        return XhaustValue::fromNull();
    }
}

XhaustValue XhaustValue::operator/(const XhaustValue &other) const
{
    if ((other.getType() == XhaustValueTypes::number || other.getType() == XhaustValueTypes::nulltype) && (getType() == XhaustValueTypes::number || getType() == XhaustValueTypes::nulltype))
    {
        return XhaustValue::fromNumber(getNumberValue() / other.getNumberValue());
    }
    else
    {
        return XhaustValue::fromNull();
    }
}

XhaustValue::operator bool() const
{
    if (getType() == XhaustValueTypes::boolean)
    {
        return valueBoolean;
    }
    else if (getType() == XhaustValueTypes::string)
    {
        return valueString != "";
    }
    else if (getType() == XhaustValueTypes::object)
    {
        return valueObject != (void*)0;
    }
    else if (getType() == XhaustValueTypes::number)
    {
        return valueNumber != 0;
    }
    else //if (getType() == XhaustValueTypes::nulltype)
    {
        return false;
    }
}

double XhaustValue::getNumberValue() const
{
    if (getType() == XhaustValueTypes::string)
    {
        return -1;
    }
    else if (getType() == XhaustValueTypes::number)
    {
        return valueNumber;
    }
    else if (getType() == XhaustValueTypes::boolean)
    {
        return valueBoolean ? 1 : 0;
    }
    else if (getType() == XhaustValueTypes::object)
    {
        uintptr_t uptr = reinterpret_cast<uintptr_t>(valueObject);
        int ptr = static_cast<int>(uptr);
        return ptr;
    }
    else //if (getType() == XhaustValueTypes::nulltype)
    {
        return -1;
    }
}

XhaustValue XhaustValue::fromNumber(double num)
{
    XhaustValue result;
    result.type = XhaustValueTypes::number;
    result.valueNumber = num;
    return result;
}

XhaustValue XhaustValue::fromNull()
{
    XhaustValue result;
    result.type = XhaustValueTypes::nulltype;
    return result;
}

XhaustValue XhaustValue::fromString(std::string str)
{
    XhaustValue result;
    result.type = XhaustValueTypes::string;
    result.valueString = str;
    return result;
}

XhaustValue XhaustValue::fromBoolean(bool val)
{
    XhaustValue result;
    result.type = XhaustValueTypes::boolean;
    result.valueBoolean = val;
    return result;
}

XhaustValue XhaustValue::fromObject(void* obj)
{
    XhaustValue result;
    result.type = XhaustValueTypes::object;
    result.valueObject = obj;
    return result;
}
#pragma endregion

#pragma region VariableStateManager
VariableStateManager::VariableStateManager()
{
    pushState();
}

void VariableStateManager::pushState()
{
    std::map<std::string, XhaustValue> newState;
    states.push_back(newState);
}

void VariableStateManager::popState()
{
    states.pop_back();
}

void VariableStateManager::setVariable(std::string key, XhaustValue value)
{
    states[states.size()-1][key] = value;
}

bool VariableStateManager::hasVariable(std::string key)
{
    return states[states.size()-1].count(key) > 0;
}

XhaustValue VariableStateManager::getVariable(std::string key)
{
    return states[states.size()-1][key];
}
#pragma endregion

#pragma region Interpreter
Interpreter* Interpreter::fromSource(std::string source)
{
    TreeGenerator *tg = TreeGenerator::fromString(source);
    std::vector<TreeNode*> baseNodes = tg->buildTree();
    delete tg;
    return new Interpreter(baseNodes);
}


//XhaustValue Interpreter::evaluateFunction(const TreeNode *functionNode, std::vector<XhaustValue> args){}

XhaustValue Interpreter::conditional(const IfNode *node)
{
    XhaustValue result = evaluate(node->condition);
    if (result.getType() == XhaustValueTypes::boolean && result)
        return evaluate(node->body);
    return result;
}

XhaustValue Interpreter::evalBlock(const BlockNode *node)
{
    XhaustValue result = XhaustValue::fromNull();
    for (TreeNode *n : node->body)
    {
        result = evaluate(n);
    }
    return result;
}

XhaustValue Interpreter::funcCall(const FunctionCallNode *node)
{
    std::vector<XhaustValue> arguments;
    for (int i = 0; i < node->functionArguments.size(); ++i)
    {
        XhaustValue result = evaluate(node->functionArguments[i]);
        arguments.push_back(result);
    }
    if (StandardFunctions::getInstance().hasFunction(node->functionName))
    {
        return StandardFunctions::getInstance().functionCall(node->functionName, arguments);
    }
    else
    {
        throw new std::exception();
    }
}

XhaustValue Interpreter::resolve(const ValueNode *node)
{
    if (!node->isConstant)
    {
        if (variableManager.hasVariable(node->value))
            return variableManager.getVariable(node->value);
        else
            return XhaustValue::fromNull();
    }
    else
    {
        if (node->value[0] == '"')
        {
            return XhaustValue::fromString(node->value.substr(1, node->value.length()-2));
        }
        else
        {
            return XhaustValue::fromNumber(std::stod(node->value));
        }
    }
}

XhaustValue Interpreter::performOperator(const OperatorNode *node)
{
    XhaustValue left = evaluate(node->lhs);
    XhaustValue right = evaluate(node->rhs);
    if (node->op == "==")
        return left == right ? XhaustValue::fromBoolean(true) : XhaustValue::fromBoolean(false);
    else if (node->op == ">=")
        return left >= right ? XhaustValue::fromBoolean(true) : XhaustValue::fromBoolean(false);
    else if (node->op == "<=")
        return left <= right ? XhaustValue::fromBoolean(true) : XhaustValue::fromBoolean(false);
    else if (node->op == "!=")
        return left != right ? XhaustValue::fromBoolean(true) : XhaustValue::fromBoolean(false);
    else if (node->op == ">")
        return left > right ? XhaustValue::fromBoolean(true) : XhaustValue::fromBoolean(false);
    else if (node->op == "<")
        return left < right ? XhaustValue::fromBoolean(true) : XhaustValue::fromBoolean(false);
    else if (node->op == "-")
        return left - right;
    else if (node->op == "+")
        return left + right;
    else if (node->op == "*")
        return left*right;
    else if (node->op == "/")
        return left/right;
    else //if (node->op == "=")
    {
        if (node->lhs->type != TN_VALUE || dynamic_cast<const ValueNode*>(node->lhs)->isConstant)
            throw new std::exception();
        std::string varName = dynamic_cast<const ValueNode*>(node->lhs)->value;
        variableManager.setVariable(varName, right);
        return right;
    }
}

XhaustValue Interpreter::exhaustOperation(const ExhaustNode *node)
{
    if (node->usesVariable)
    {
        std::string variableName = reinterpret_cast<const ValueNode*>(node->condition)->value;
        XhaustValue variable = variableManager.getVariable(variableName);
        XhaustValue result;
        while (variableManager.getVariable(variableName).getNumberValue() > 0)
        {
            result = evaluate(node->body);
            int counter = (int)(variableManager.getVariable(variableName).getNumberValue()+0.5);
            counter--;
            variableManager.setVariable(variableName, XhaustValue::fromNumber(counter));
        }
        return result;
    }
    else
    {
        XhaustValue val = evaluate(node->condition);
        int counter = (int)val.getNumberValue();
        XhaustValue result;
        while (counter > 0)
        {
            result = evaluate(node->body);
            counter--;
        }
        return result;
    }
}

XhaustValue Interpreter::evaluate(const TreeNode *node)
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
    else if (node->type == TN_EXHAUST)
        return exhaustOperation(dynamic_cast<const ExhaustNode*>(node));
    else //if (node->type == TN_OPERATOR)
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

XhaustValue Interpreter::start()
{
    XhaustValue result = XhaustValue::fromNull();
    for (TreeNode* baseNode : baseNodes)
    {
        result = evaluate(baseNode);
    }
    return result;
}
#pragma endregion