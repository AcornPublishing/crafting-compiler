#include <iostream>
#include "Node.h"

using std::cout;
using std::endl;

static auto indent(int)->void;

auto printSyntaxTree(Program* program)->void {
  for (auto& node: program->functions)
    node->print(0);
}

auto Function::print(int depth)->void {
  indent(depth); cout << "FUNCTION " << name << ": " << endl;
  if (parameters.size()) {
    indent(depth + 1); cout << "PARAMETERS:";
    for (auto& name: parameters)
      cout << name << " ";
    cout << endl;
  }
  indent(depth + 1); cout << "BLOCK:" << endl;
  for (auto& node: block)
    node->print(depth + 2);
}

auto For::print(int depth)->void {
  indent(depth); cout << "FOR:" << endl;
  indent(depth + 1); cout << "VARIABLE:" << endl;
  variable->print(depth + 2);
  indent(depth + 1); cout << "CONDITION:" << endl;
  condition->print(depth + 2);
  indent(depth + 1); cout << "EXPRESSION:" << endl;
  expression->print(depth + 2);
  indent(depth + 1); cout << "BLOCK:" << endl;
  for (auto& node: block)
    node->print(depth + 2);
}

auto If::print(int depth)->void {
  for (size_t i = 0; i < conditions.size(); i++) {
    indent(depth); cout << (i == 0 ? "IF:" : "ELIF:") << endl;
    indent(depth + 1); cout << "CONDITION:" << endl;
    conditions[i]->print(depth + 2);
    indent(depth + 1); cout << "BLOCK:" << endl;
    for (auto& node: blocks[i])
      node->print(depth + 2);
  }
  if (elseBlock.size() == 0)
    return;
  indent(depth); cout << "ELSE:" << endl;
  for (auto& node: elseBlock)
    node->print(depth + 1);
}

auto Variable::print(int depth)->void {
  indent(depth); cout << "VAR " << name << ":" << endl;
  expression->print(depth + 1);
}

auto Print::print(int depth)->void {
  indent(depth); cout << (lineFeed ? "PRINT_LINE" : "PRINT:") << endl;
  for (auto& node: arguments)
    node->print(depth + 1);
}

auto Return::print(int depth)->void {
  indent(depth); cout << "RETURN:" << endl;
  expression->print(depth + 1);
}

auto Break::print(int depth)->void {
  indent(depth); cout << "BREAK" << endl;
}

auto Continue::print(int depth)->void {
  indent(depth); cout << "CONTINUE" << endl;
}

auto ExpressionStatement::print(int depth)->void {
  indent(depth); cout << "EXPRESSION:" << endl;
  expression->print(depth + 1);
}

auto Or::print(int depth)->void {
  indent(depth); cout << "OR:" << endl;
  indent(depth + 1); cout << "LHS:" << endl;
  lhs->print(depth + 2);
  indent(depth + 1); cout << "RHS:" << endl;
  rhs->print(depth + 2);
}

auto And::print(int depth)->void {
  indent(depth); cout << "AND:" << endl;
  indent(depth + 1); cout << "LHS:" << endl;
  lhs->print(depth + 2);
  indent(depth + 1); cout << "RHS:" << endl;
  rhs->print(depth + 2);
}

auto Relational::print(int depth)->void {
  indent(depth); cout << toString(kind) << ":" << endl;
  indent(depth + 1); cout << "LHS:" << endl;
  lhs->print(depth + 2);
  indent(depth + 1); cout << "RHS:" << endl;
  rhs->print(depth + 2);
}

auto Arithmetic::print(int depth)->void {
  indent(depth); cout << toString(kind) << ":" << endl;
  indent(depth + 1); cout << "LHS:" << endl;
  lhs->print(depth + 2);
  indent(depth + 1); cout << "RHS:" << endl;
  rhs->print(depth + 2);
}

auto Unary::print(int depth)->void {
  indent(depth); cout << toString(kind) << endl;
  sub->print(depth + 1);
}

auto GetElement::print(int depth)->void {
  indent(depth); cout << "GET_ELEMENT:" << endl;
  indent(depth + 1); cout << "SUB:" << endl;
  sub->print(depth + 2);
  indent(depth + 1); cout << "INDEX:" << endl;
  index->print(depth + 2);
}

auto SetElement::print(int depth)->void {
  indent(depth); cout << "SET_ELEMENT:" << endl;
  indent(depth + 1); cout << "SUB:" << endl;
  sub->print(depth + 2);
  indent(depth + 1); cout << "INDEX:" << endl;
  index->print(depth + 2);
  indent(depth + 1); cout << "VALUE:" << endl;
  value->print(depth + 2);
}

auto Call::print(int depth)->void {
  indent(depth); cout << "CALL:" << endl;
  indent(depth + 1); cout << "EXPRESSION:" << endl;
  sub->print(depth + 2);
  for (auto& node: arguments) {
    indent(depth + 1); cout << "ARGUMENT:" << endl;
    node->print(depth + 2);
  }
}

auto NullLiteral::print(int depth)->void {
  indent(depth); cout << "null" << endl;
}

auto BooleanLiteral::print(int depth)->void {
  indent(depth); cout << (value ? "true" : "false") << endl;
}

auto NumberLiteral::print(int depth)->void {
  indent(depth); cout << value << endl;
}

auto StringLiteral::print(int depth)->void {
  indent(depth); cout << "\"" << value << "\"" << endl;
}

auto ArrayLiteral::print(int depth)->void {
  indent(depth); cout << "[" << endl;
  for (auto& node: values)
    node->print(depth + 1);
  indent(depth); cout << "]" << endl;
}

auto MapLiteral::print(int depth)->void {
  indent(depth); cout << "{" << endl;
  for (auto& [key, value]: values) {
    cout << key << ": ";
    value->print(depth + 1);
  }
  indent(depth); cout << "}" << endl;
}

auto GetVariable::print(int depth)->void {
  indent(depth); cout << "GET_VARIABLE: " << name << endl;
}

auto SetVariable::print(int depth)->void {
  indent(depth); cout << "SET_VARIABLE: " << name << endl;
  value->print(depth + 1);
}

auto indent(int depth)->void {
  cout << string(static_cast<size_t>(depth * 2), ' ');
}
