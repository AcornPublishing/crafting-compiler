#pragma once
#include <string>
#include <iostream>

using std::string;
using std::ostream;

enum class Kind {
  Unknown, EndOfToken,

  NullLiteral,
  TrueLiteral, FalseLiteral,
  NumberLiteral, StringLiteral,
  Identifier,

  Function, Return,
  Variable,
  For, Break, Continue,
  If, Elif, Else,
  Print, PrintLine,

  LogicalAnd, LogicalOr,
  Assignment,
  Add, Subtract,
  Multiply, Divide, Modulo,
  Equal, NotEqual,
  LessThan, GreaterThan,
  LessOrEqual, GreaterOrEqual,

  Comma, Colon, Semicolon,
  LeftParen, RightParen,
  LeftBrace, RightBrace,
  LeftBraket, RightBraket,
};

auto toKind(string)->Kind;
auto toString(Kind)->string;

struct Token {
  Kind kind = Kind::Unknown;
  string string;
};

auto operator<<(ostream&, Token&)->ostream&;
