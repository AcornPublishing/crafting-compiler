#pragma once
#include <any>
#include <map>
#include <vector>
#include <string>
#include "Token.h"

using std::any;
using std::map;
using std::vector;
using std::string;

struct Program {
  vector<struct Function*> functions;
};

struct Statement {
  virtual auto interpret()->void = 0;
};

struct Expression {
  virtual auto interpret()->any = 0;
};

struct Function: Statement {
  string name;
  vector<string> parameters;
  vector<Statement*> block;
  auto interpret()->void;
};

struct Variable: Statement {
  string name;
  Expression* expression;
  auto interpret()->void;
};

struct Return: Statement {
  Expression* expression;
  auto interpret()->void;
};

struct For: Statement {
  Variable* variable;
  Expression* condition;
  Expression* expression;
  vector<Statement*> block;
  auto interpret()->void;
};

struct Break: Statement {
  auto interpret()->void;
};

struct Continue: Statement {
  auto interpret()->void;
};

struct If: Statement {
  vector<Expression*> conditions;
  vector<vector<Statement*>> blocks;
  vector<Statement*> elseBlock;
  auto interpret()->void;
};

struct Print: Statement {
  bool lineFeed = false;
  vector<Expression*> arguments;
  auto interpret()->void;
};

struct ExpressionStatement: Statement {
  Expression* expression;
  auto interpret()->void;
};

struct Or: Expression {
  Expression* lhs;
  Expression* rhs;
  auto interpret()->any;
};

struct And: Expression {
  Expression* lhs;
  Expression* rhs;
  auto interpret()->any;
};

struct Relational: Expression {
  Kind kind;
  Expression* lhs;
  Expression* rhs;
  auto interpret()->any;
};

struct Arithmetic: Expression {
  Kind kind;
  Expression* lhs;
  Expression* rhs;
  auto interpret()->any;
};

struct Unary: Expression {
  Kind kind;
  Expression* sub;
  auto interpret()->any;
};

struct Call: Expression {
  Expression* sub;
  vector<Expression*> arguments;
  auto interpret()->any;
};

struct GetElement: Expression {
  Expression* sub;
  Expression* index;
  auto interpret()->any;
};

struct SetElement: Expression {
  Expression* sub;
  Expression* index;
  Expression* value;
  auto interpret()->any;
};

struct GetVariable: Expression {
  string name;
  auto interpret()->any;
};

struct SetVariable: Expression {
  string name;
  Expression* value;
  auto interpret()->any;
};

struct NullLiteral: Expression {
  auto interpret()->any;
};

struct BooleanLiteral: Expression {
  bool value = false;
  auto interpret()->any;
};

struct NumberLiteral: Expression {
  double value = 0.0;
  auto interpret()->any;
};

struct StringLiteral: Expression {
  string value;
  auto interpret()->any;
};

struct ArrayLiteral: Expression {
  vector<Expression*> values;
  auto interpret()->any;
};

struct MapLiteral: Expression {
  map<string, Expression*> values;
  auto interpret()->any;
};
