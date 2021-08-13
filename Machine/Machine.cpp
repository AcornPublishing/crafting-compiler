#include <any>
#include <map>
#include <list>
#include <vector>
#include <functional>
#include "Datatype.h"
#include "Code.h"

using std::get;
using std::any;
using std::map;
using std::cout;
using std::endl;
using std::list;
using std::tuple;
using std::vector;
using std::function;

struct StackFrame {
  vector<any> variables;
  vector<any> operandStack;
  size_t instructionPointer = 0;
};
static list<Object*> objects;
static map<string, any> global;
static vector<StackFrame> callStack;
extern map<string, function<any(vector<any>)>> builtinFunctionTable;

static auto pushOperand(any value)->void;
static auto peekOperand()->any;
static auto popOperand()->any;
static auto collectGarbage()->void;
static auto markObject(any)->void;
static auto sweepObject()->void;

auto execute(tuple<vector<Code>, map<string, size_t>> objectCode)->void {
  global.clear();
  objects.clear();
  callStack.emplace_back();
  auto codeList = get<0>(objectCode);
  auto functionTable = get<1>(objectCode);
  while (true) {
    auto code = codeList[callStack.back().instructionPointer];
    switch (code.instruction) {
    case Instruction::Exit: {
      callStack.pop_back();
      return;
    }
    case Instruction::Call: {
      auto operand = popOperand();
      if (isSize(operand)) {
        StackFrame stackFrame;
        stackFrame.instructionPointer = toSize(operand);
        for (size_t i = 0; i < toSize(code.operand); i++) {
          stackFrame.variables.push_back(callStack.back().operandStack.back());
          callStack.back().operandStack.pop_back();
        }
        callStack.push_back(stackFrame);
        continue;
      }
      if (isBuiltinFunction(operand)) {
        vector<any> arguments;
        for (size_t i = 0; i < toSize(code.operand); i++)
          arguments.push_back(popOperand());
        pushOperand(toBuiltinFunction(operand)(arguments));
        break;
      }
      pushOperand(nullptr);
      break;
    }
    case Instruction::Alloca: {
      auto extraSize = toSize(code.operand);
      auto currentSize = callStack.back().variables.size();
      callStack.back().variables.resize(currentSize + extraSize);
      break;
    }
    case Instruction::Return: {
      any result = nullptr;
      if (callStack.back().operandStack.empty() == false)
        result = callStack.back().operandStack.back();
      callStack.pop_back();
      callStack.back().operandStack.push_back(result);
      collectGarbage();
      break;
    }
    case Instruction::Jump: {
      callStack.back().instructionPointer = toSize(code.operand);
      continue;
    }
    case Instruction::ConditionJump: {
      auto condition = popOperand();
      if (isTrue(condition))
        break;
      callStack.back().instructionPointer = toSize(code.operand);
      continue;
    }
    case Instruction::Print: {
      for (size_t i = 0; i < toSize(code.operand); i++) {
        auto value = popOperand();
        cout << value;
      }
      break;
    }
    case Instruction::PrintLine: {
      cout << endl;
      break;
    }
    case Instruction::LogicalOr: {
      auto value = popOperand();
      if (isTrue(value)) {
        pushOperand(value);
        callStack.back().instructionPointer = toSize(code.operand);
        continue;
      }
      break;
    }
    case Instruction::LogicalAnd: {
      auto value = popOperand();
      if (isFalse(value)) {
        pushOperand(value);
        callStack.back().instructionPointer = toSize(code.operand);
        continue;
      }
      break;
    }
    case Instruction::Equal: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNull(lValue) && isNull(rValue))
        pushOperand(true);
      else if (isBoolean(lValue) && isBoolean(rValue))
        pushOperand(toBoolean(lValue) == toBoolean(rValue));
      else if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) == toNumber(rValue));
      else if (isString(lValue) && isString(rValue))
        pushOperand(toString(lValue) == toString(rValue));
      else
        pushOperand(false);
      break;
    }
    case Instruction::NotEqual: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNull(lValue) && isNull(rValue))
        pushOperand(false);
      else if (isNull(lValue) || isNull(rValue))
        pushOperand(true);
      if (isBoolean(lValue) && isBoolean(rValue))
        pushOperand(toBoolean(lValue) != toBoolean(rValue));
      else if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) != toNumber(rValue));
      else if (isString(lValue) && isString(rValue))
        pushOperand(toString(lValue) != toString(rValue));
      else
        pushOperand(false);
      break;
    }
    case Instruction::LessThan: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) < toNumber(rValue));
      else
        pushOperand(false);
      break;
    }
    case Instruction::GreaterThan: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) > toNumber(rValue));
      else
        pushOperand(false);
      break;
    }
    case Instruction::LessOrEqual: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) <= toNumber(rValue));
      else
        pushOperand(false);
      break;
    }
    case Instruction::GreaterOrEqual: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) >= toNumber(rValue));
      else
        pushOperand(false);
      break;
    }
    case Instruction::Add: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) + toNumber(rValue));
      else if (isString(lValue) && isString(rValue))
        pushOperand(toString(lValue) + toString(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Subtract: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) - toNumber(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Multiply: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) * toNumber(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Divide: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue) && toNumber(rValue) == 0)
        pushOperand(0.0);
      else if (isNumber(lValue) && isNumber(rValue))
        pushOperand(toNumber(lValue) / toNumber(rValue));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Modulo: {
      auto rValue = popOperand();
      auto lValue = popOperand();
      if (isNumber(lValue) && isNumber(rValue) && toNumber(rValue) == 0)
        pushOperand(0.0);
      else if (isNumber(lValue) && isNumber(rValue))
        pushOperand(fmod(toNumber(lValue), toNumber(rValue)));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::Absolute: {
      auto value = popOperand();
      if (isNumber(value))
        pushOperand(abs(toNumber(value)));
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::ReverseSign: {
      auto value = popOperand();
      if (isNumber(value))
        pushOperand(toNumber(value) * -1);
      else
        pushOperand(0.0);
      break;
    }
    case Instruction::GetElement: {
      auto index = popOperand();
      auto sub = popOperand();
      if (isArray(sub) && isNumber(index))
        pushOperand(getValueOfArray(sub, index));
      else if (isMap(sub) && isString(index))
        pushOperand(getValueOfMap(sub, index));
      else
        pushOperand(nullptr);
      break;
    }
    case Instruction::SetElement: {
      auto index = popOperand();
      auto sub = popOperand();
      if (isArray(sub) && isNumber(index))
        setValueOfArray(sub, index, peekOperand());
      else if (isMap(sub) && isString(index))
        setValueOfMap(sub, index, peekOperand());
      break;
    }
    case Instruction::GetGlobal: {
      auto name = toString(code.operand);
      if (functionTable.count(name))
        pushOperand(functionTable[name]);
      else if (builtinFunctionTable.count(name))
        pushOperand(builtinFunctionTable[name]);
      else if (global.count(name))
        pushOperand(global[name]);
      else
        pushOperand(nullptr);
      break;
    }
    case Instruction::SetGlobal: {
      auto name = toString(code.operand);
      global[name] = peekOperand();
      break;
    }
    case Instruction::GetLocal: {
      auto index = toSize(code.operand);
      pushOperand(callStack.back().variables[index]);
      break;
    }
    case Instruction::SetLocal: {
      auto index = toSize(code.operand);
      callStack.back().variables[index] = peekOperand();
      break;
    }
    case Instruction::PushNull: {
      pushOperand(nullptr);
      break;
    }
    case Instruction::PushBoolean: {
      pushOperand(code.operand);
      break;
    }
    case Instruction::PushNumber: {
      pushOperand(code.operand);
      break;
    }
    case Instruction::PushString: {
      pushOperand(code.operand);
      break;
    }
    case Instruction::PushArray: {
      auto result = new Array();
      auto size = toSize(code.operand);
      for (auto i = size; i > 0; i--)
        result->values.push_back(popOperand());
      pushOperand(result);
      objects.push_back(result);
      break;
    }
    case Instruction::PushMap: {
      auto result = new Map();
      for (size_t i = 0; i < toSize(code.operand); i++) {
        auto value = popOperand();
        auto key = toString(popOperand());
        result->values[key] = value;
      }
      pushOperand(result);
      objects.push_back(result);
      break;
    }
    case Instruction::PopOperand: {
      popOperand();
      break;
    }
    }
    callStack.back().instructionPointer += 1;
  }
}

auto pushOperand(any value)->void {
  callStack.back().operandStack.push_back(value);
}

auto peekOperand()->any {
  return callStack.back().operandStack.back();
}

auto popOperand()->any {
  auto value = callStack.back().operandStack.back();
  callStack.back().operandStack.pop_back();
  return value;
}

auto collectGarbage()->void {
  for (auto& stackFrame: callStack) {
    for (auto& value: stackFrame.operandStack)
      markObject(value);
    for (auto& value: stackFrame.variables)
      markObject(value);
  }
  for (auto& [key, value]: global)
    markObject(value);
  sweepObject();
}

auto markObject(any value)->void {
  if (isArray(value)) {
    if (toArray(value)->isMarked)
      return;
    toArray(value)->isMarked = true;
    for (auto& value: toArray(value)->values)
      markObject(value);
  }
  else if (isMap(value)) {
    if (toMap(value)->isMarked)
      return;
    toMap(value)->isMarked = true;
    for (auto& [key, value]: toMap(value)->values)
      markObject(value);
  }
}

auto sweepObject()->void {
  objects.remove_if([](Object* object) {
    if (object->isMarked) {
      object->isMarked = false;
      return false;
    }
    delete object;
    return true;
  });
}
