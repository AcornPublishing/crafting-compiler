#include <map>
#include <any>
#include <list>
#include <vector>
#include <functional>
#include "Datatype.h"
#include "Node.h"

using std::any;
using std::map;
using std::list;
using std::cout;
using std::endl;
using std::vector;
using std::function;

struct ReturnException { any result; };
struct BreakException {};
struct ContinueException {};

static map<string, any> global;
static list<list<map<string, any>>> local;
static map<string, Function*> functionTable;
extern map<string, function<any(vector<any>)>> builtinFunctionTable;

auto interpret(Program* program)->void {
  functionTable.clear();
  global.clear();
  local.clear();
  for (auto& node: program->functions)
    functionTable[node->name] = node;
  if (functionTable["main"] == nullptr)
    return;
  local.emplace_back().emplace_front();
  try {
    functionTable["main"]->interpret();
  }
  catch (ReturnException) {}
  catch (BreakException) {}
  catch (ContinueException) {}
  local.pop_back();
}

auto Function::interpret()->void {
  for (auto& node: block)
    node->interpret();
}

auto Return::interpret()->void {
  throw ReturnException{expression->interpret()};
}

auto Variable::interpret()->void {
  local.back().front()[name] = expression->interpret();
}

auto For::interpret()->void {
  local.back().emplace_front();
  variable->interpret();
  while (true) {
    auto result = condition->interpret();
    if (isTrue(result) == false)
      break;
    try {
      for (auto& node: block)
        node->interpret();
    } catch (ContinueException) {
      /* empty */
    } catch (BreakException) {
      break;
    }
    expression->interpret();
  }
  local.back().pop_front();
}

auto Break::interpret()->void {
  throw BreakException();
}

auto Continue::interpret()->void {
  throw ContinueException();
}

auto If::interpret()->void {
  for (size_t i = 0; i < conditions.size(); i++) {
    auto result = conditions[i]->interpret();
    if (isTrue(result) == false)
      continue;
    local.back().emplace_front();
    for (auto& node: blocks[i])
      node->interpret();
    local.back().pop_front();
    return;
  }
  if (elseBlock.empty())
    return;
  local.back().emplace_front();
  for (auto& node: elseBlock)
    node->interpret();
  local.back().pop_front();
}

auto Print::interpret()->void {
  for (auto& node: arguments) {
    auto value = node->interpret();
    cout << value;
  }
  if (lineFeed) cout << endl;
}

auto ExpressionStatement::interpret()->void {
  expression->interpret();
}

auto Or::interpret()->any {
  return isTrue(lhs->interpret()) ? true : rhs->interpret();
}

auto And::interpret()->any {
  return isFalse(lhs->interpret()) ? false : rhs->interpret();
}

auto Relational::interpret()->any {
  auto lValue = lhs->interpret();
  auto rValue = rhs->interpret();
  if (kind == Kind::Equal && isNull(lValue) && isNull(rValue)) {
    return true;
  }
  if (kind == Kind::Equal && isBoolean(lValue) && isBoolean(rValue)) {
    return toBoolean(lValue) == toBoolean(rValue);
  }
  if (kind == Kind::Equal && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) == toNumber(rValue);
  }
  if (kind == Kind::Equal && isString(lValue) && isString(rValue)) {
    return toString(lValue) == toString(rValue);
  }
  if (kind == Kind::NotEqual && isNull(lValue) && isNull(rValue)) {
    return false;
  }
  if (kind == Kind::NotEqual && isNull(lValue) || isNull(rValue)) {
    return true;
  }
  if (kind == Kind::NotEqual && isBoolean(lValue) && isBoolean(rValue)) {
    return toBoolean(lValue) != toBoolean(rValue);
  }
  if (kind == Kind::NotEqual && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) != toNumber(rValue);
  }
  if (kind == Kind::NotEqual && isString(lValue) && isString(rValue)) {
    return toString(lValue) != toString(rValue);
  }
  if (kind == Kind::LessThan && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) < toNumber(rValue);
  }
  if (kind == Kind::GreaterThan && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) > toNumber(rValue);
  }
  if (kind == Kind::LessOrEqual && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) <= toNumber(rValue);
  }
  if (kind == Kind::GreaterOrEqual && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) >= toNumber(rValue);
  }
  return false;
}

auto Arithmetic::interpret()->any {
  auto lValue = lhs->interpret();
  auto rValue = rhs->interpret();
  if (kind == Kind::Add && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) + toNumber(rValue);
  }
  if (kind == Kind::Add && isString(lValue) && isString(rValue)) {
    return toString(lValue) + toString(rValue);
  }
  if (kind == Kind::Subtract && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) - toNumber(rValue);
  }
  if (kind == Kind::Multiply && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(lValue) * toNumber(rValue);
  }
  if (kind == Kind::Divide && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(rValue) == 0 ? 0.0 : toNumber(lValue) / toNumber(rValue);
  }
  if (kind == Kind::Modulo && isNumber(lValue) && isNumber(rValue)) {
    return toNumber(rValue) == 0 ? toNumber(lValue) : fmod(toNumber(lValue), toNumber(rValue));
  }
  return 0.0;
}

auto Unary::interpret()->any {
  auto value = sub->interpret();
  if (kind == Kind::Add && isNumber(value))
    return abs(toNumber(value));
  if (kind == Kind::Subtract && isNumber(value))
    return toNumber(value) * -1;
  return 0.0;
}

auto Call::interpret()->any {
  auto value = sub->interpret();
  if (isBuiltinFunction(value)) {
    vector<any> values;
    for (size_t i = 0; i < arguments.size(); i++)
      values.push_back(arguments[i]->interpret());
    return toBuiltinFunction(value)(values);
  }
  if (isFunction(value) == false)
    return nullptr;
  map<string, any> parameters;
  for (size_t i = 0; i < arguments.size(); i++) {
    auto name = toFunction(value)->parameters[i];
    parameters[name] = arguments[i]->interpret();
  }
  local.emplace_back().push_front(parameters);
  try {
    toFunction(value)->interpret();
  } catch (ReturnException exception) {
    local.pop_back();
    return exception.result;
  }
  local.pop_back();
  return nullptr;
}

auto GetElement::interpret()->any {
  auto object = sub->interpret();
  auto index_ = index->interpret();
  if (isArray(object) && isNumber(index_))
    return getValueOfArray(object, index_);
  if (isMap(object) && isString(index_))
    return getValueOfMap(object, index_);
  return nullptr;
}

auto SetElement::interpret()->any {
  auto object = sub->interpret();
  auto index_ = index->interpret();
  auto value_ = value->interpret();
  if (isArray(object) && isNumber(index_))
    return setValueOfArray(object, index_, value_);
  if (isMap(object) && isString(index_))
    return setValueOfMap(object, index_, value_);
  return nullptr;
}

auto GetVariable::interpret()->any {
  for (auto& variables: local.back()) {
    if (variables.count(name))
      return variables[name];
  }
  if (global.count(name))
    return global[name];
  if (functionTable.count(name))
    return functionTable[name];
  if (builtinFunctionTable.count(name))
    return builtinFunctionTable[name];
  return nullptr;
}

auto SetVariable::interpret()->any {
  for (auto& variables: local.back()) {
    if (variables.count(name))
      return variables[name] = value->interpret();
  }
  return global[name] = value->interpret();
}

auto NullLiteral::interpret()->any {
  return nullptr;
}

auto BooleanLiteral::interpret()->any {
  return value;
}

auto NumberLiteral::interpret()->any {
  return value;
}

auto StringLiteral::interpret()->any {
  return value;
}

auto ArrayLiteral::interpret()->any {
  auto result = new Array();
  for (auto& node: values)
    result->values.push_back(node->interpret());
  return result;
}

auto MapLiteral::interpret()->any {
  auto result = new Map();
  for (auto& [key, value]: values)
    result->values.insert_or_assign(key, value->interpret());
  return result;
}
