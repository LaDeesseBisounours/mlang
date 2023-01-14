#include "parser.hpp"
#include "parser_handler.hpp"
#include "scanner.hpp"
#include <iostream>

using namespace ParserLayer;
using namespace std;

int main(int argc, char **argv) {
    ParserHandler i;
    int res = i.parse();
    cout << "Parse complete. Result = " << res << endl;
    return res;
}
