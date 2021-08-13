#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include "Token.h"

using std::cout;
using std::endl;
using std::setw;
using std::left;
using std::string;
using std::vector;

auto scan(string)->vector<Token>;
auto printTokenList(vector<Token>)->void;
