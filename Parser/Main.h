#pragma once
#include "Token.h"
#include "Node.h"

using std::string;

auto printSyntaxTree(Program*)->void;
auto scan(string)->vector<Token>;
auto parse(vector<Token>)->Program*;
