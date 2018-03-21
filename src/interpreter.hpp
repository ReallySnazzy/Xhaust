#ifndef interpreter_hpp
#define interpreter_hpp 1

#include "treegen.hpp"
#include <map>
#include <functional>
#include <cstdint>

enum class XhaustValueTypes
{
  number,
  string,
  object,
  boolean,
  nulltype
};

// A class that represents an xhaust value.
class XhaustValue
{
private:
  XhaustValueTypes type = XhaustValueTypes::nulltype;

  double valueNumber;
  std::string valueString;
  bool valueBoolean;
  void *valueObject;

public:
  XhaustValue() = default;

  XhaustValueTypes getType() const;

  static XhaustValue Boolean(bool);
  static XhaustValue Number(double);
  static XhaustValue String(std::string);
  static XhaustValue Object(void *);
  static XhaustValue Null();

  XhaustValue operator==(const XhaustValue &other) const;
  XhaustValue operator>=(const XhaustValue &other) const;
  XhaustValue operator<=(const XhaustValue &other) const;
  XhaustValue operator>(const XhaustValue &other) const;
  XhaustValue operator<(const XhaustValue &other) const;

  XhaustValue operator+(const XhaustValue &other) const;
  XhaustValue operator-(const XhaustValue &other) const;
  XhaustValue operator/(const XhaustValue &other) const;
  XhaustValue operator*(const XhaustValue &other) const;

  operator bool() const;

  std::string toString() const;
  double getNumberValue() const;
  void *getObjectValue() const { return valueObject; }
};

// All variables are stored internally as integers.
class VariableStateManager
{
private:
  std::vector<std::map<std::string, XhaustValue>> states;

public:
  // Starts with an empty state.
  VariableStateManager();

  // Creates a new empty variable table and stores the previous one for later.
  void pushState();

  // Gets rid of the current state and restores the previous one.
  void popState();

  XhaustValue getVariable(std::string name);
  bool hasVariable(std::string name);
  void setVariable(std::string name, XhaustValue value);
};

class Interpreter
{
private:
  std::vector<TreeNode *> baseNodes;
  VariableStateManager variableManager;

  Interpreter(std::vector<TreeNode *> nodes);

  XhaustValue resolve(const ValueNode *);
  XhaustValue funcCall(const FunctionCallNode *);
  XhaustValue conditional(const IfNode *);
  XhaustValue evalBlock(const BlockNode *);

public:
  virtual ~Interpreter();
  static Interpreter *fromSource(std::string source);
  XhaustValue evaluate(const TreeNode *node);
  XhaustValue evaluateFunction(const TreeNode *functionNode, std::vector<XhaustValue> args);
  XhaustValue performOperator(const OperatorNode *opNode);
  XhaustValue exhaustOperation(const ExhaustNode *ex);
  XhaustValue start();
};

class StandardFunctions
{
private:
  static StandardFunctions instance;
  std::map<std::string, std::function<XhaustValue(std::vector<XhaustValue>)>> callbacks;
  StandardFunctions();

public:
  virtual ~StandardFunctions(){};
  static StandardFunctions &getInstance();
  bool hasFunction(std::string name);
  XhaustValue functionCall(std::string name, std::vector<XhaustValue> args);
};

#endif // interpreter_hpp
