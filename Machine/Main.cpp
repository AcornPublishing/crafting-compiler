#include "Main.h"

auto main(int argc, char** argv)->int {  
  string sourceCode = R""""(
    function main() {
      print 'Hello, World!';
    }
  )"""";
  auto tokenList = scan(sourceCode);
  auto syntaxTree = parse(tokenList);
  auto objectCode = generate(syntaxTree);
  execute(objectCode);
  return 0;
}
