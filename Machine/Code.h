#pragma once
#include <any>
#include <string>
#include <iostream>

using std::any;
using std::string;
using std::ostream;

enum class Instruction {
  Exit,
  Call, Alloca, Return,
  Jump, ConditionJump,
  Print, PrintLine,

  LogicalOr, LogicalAnd,
  Add, Subtract,
  Multiply, Divide, Modulo,
  Equal, NotEqual,
  LessThan, GreaterThan,
  LessOrEqual, GreaterOrEqual,
  Absolute, ReverseSign,

  GetElement, SetElement,
  GetGlobal, SetGlobal,
  GetLocal, SetLocal,

  PushNull, PushBoolean,
  PushNumber, PushString,
  PushArray, PushMap,
  PopOperand,
};

auto toString(Instruction)->string;

struct Code {
  Instruction instruction;
  any operand;
};

auto operator<<(ostream&, Code&)->ostream&;
