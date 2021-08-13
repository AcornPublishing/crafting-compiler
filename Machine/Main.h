#pragma once
#include <tuple>
#include "Token.h"
#include "Node.h"
#include "Code.h"

using std::tuple;

auto scan(string)->vector<Token>;
auto parse(vector<Token>)->Program*;
auto generate(Program*)->tuple<vector<Code>, map<string, size_t>>;
auto execute(tuple<vector<Code>, map<string, size_t>>)->void;
