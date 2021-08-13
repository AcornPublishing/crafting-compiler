#pragma once
#include <any>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include "Node.h"

using std::any;
using std::map;
using std::vector;
using std::string;
using std::ostream;
using std::function;

struct Object {
  bool isMarked = false;
  virtual ~Object() {}
};

struct Array: Object {
  vector<any> values;
};

struct Map: Object {
  map<string, any> values;
};

auto isSize(any value)->bool;
auto toSize(any value)->size_t;

auto isNull(any value)->bool;

auto isTrue(any value)->bool;
auto isFalse(any value)->bool;
auto isBoolean(any value)->bool;
auto toBoolean(any value)->bool;

auto isNumber(any value)->bool;
auto toNumber(any value)->double;

auto isString(any value)->bool;
auto toString(any value)->string;

auto isArray(any value)->bool;
auto toArray(any value)->Array*;
auto getValueOfArray(any object, any index)->any;
auto setValueOfArray(any object, any index, any value)->any;

auto isMap(any value)->bool;
auto toMap(any value)->Map*;
auto getValueOfMap(any object, any key)->any;
auto setValueOfMap(any object, any key, any value)->any;

auto isFunction(any)->bool;
auto toFunction(any)->Function*;

auto isBuiltinFunction(any value)->bool;
auto toBuiltinFunction(any value)->function<any(vector<any>)>;

auto operator<<(ostream& stream, any& value)->ostream&;
