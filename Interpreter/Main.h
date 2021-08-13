#pragma once
#include "Token.h"
#include "Node.h"

auto scan(string)->vector<Token>;
auto parse(vector<Token>)->Program*;
auto interpret(Program*)->void;
