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
  virtual auto generate()->void = 0;
};

struct Expression {
  virtual auto generate()->void = 0;
};

struct Function: Statement {
  string name;
  vector<string> parameters;
  vector<Statement*> block;
  auto generate()->void;
};

struct Variable: Statement {
  string name;
  Expression* expression;
  auto generate()->void;
};

struct Return: Statement {
  Expression* expression;
  auto generate()->void;
};

struct For: Statement {
  Variable* variable;
  Expression* condition;
  Expression* expression;
  vector<Statement*> block;
  auto generate()->void;
};

struct Break: Statement {
  auto generate()->void;
};

struct Continue: Statement {
  auto generate()->void;
};

struct If: Statement {
  vector<Expression*> conditions;
  vector<vector<Statement*>> blocks;
  vector<Statement*> elseBlock;
  auto generate()->void;
};

struct Print: Statement {
  bool lineFeed = false;
  vector<Expression*> arguments;
  auto generate()->void;
};

struct ExpressionStatement: Statement {
  Expression* expression;
  auto generate()->void;
};

struct Or: Expression {
  Expression* lhs;
  Expression* rhs;
  auto generate()->void;
};

struct And: Expression {
  Expression* lhs;
  Expression* rhs;
  auto generate()->void;
};

struct Relational: Expression {
  Kind kind;
  Expression* lhs;
  Expression* rhs;
  auto generate()->void;
};

struct Arithmetic: Expression {
  Kind kind;
  Expression* lhs;
  Expression* rhs;
  auto generate()->void;
};

struct Unary: Expression {
  Kind kind;
  Expression* sub;
  auto generate()->void;
};

struct Call: Expression {
  Expression* sub;
  vector<Expression*> arguments;
  auto generate()->void;
};

struct GetElement: Expression {
  Expression* sub;
  Expression* index;
  auto generate()->void;
};

struct SetElement: Expression {
  Expression* sub;
  Expression* index;
  Expression* value;
  auto generate()->void;
};

struct GetVariable: Expression {
  string name;
  auto generate()->void;
};

struct SetVariable: Expression {
  string name;
  Expression* value;
  auto generate()->void;
};

struct NullLiteral: Expression {
  auto generate()->void;
};

struct BooleanLiteral: Expression {
  bool value = false;
  auto generate()->void;
};

struct NumberLiteral: Expression {
  double value = 0.0;
  auto generate()->void;
};

struct StringLiteral: Expression {
  string value;
  auto generate()->void;
};

struct ArrayLiteral: Expression {
  vector<Expression*> values;
  auto generate()->void;
};

struct MapLiteral: Expression {
  map<string, Expression*> values;
  auto generate()->void;
};
