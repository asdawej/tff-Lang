#include "tff.h"
#include <fstream>
#include <iostream>
using namespace std;

// ******

void add() { tff::stack[-1] = tff::stack[1] + tff::stack[2]; }

int main() {
    Register::dict_Key2ObjectStd[Register::ID_ObjectStd::Key_add] = (void *)add;
    Function::StreamIONode n1;
    Function::StdFunctionNode n2;
    Function::StreamIONode n3;
    Function::EndNode n0;
    n1.size = 2;
    n1.str_O = &Stream::stdinStream;
    n1.str_I = new Stream::StackStream(1);
    n2.func = add;
    n3.size = 1;
    n3.str_O = new Stream::StackStream(-1);
    n3.str_I = &Stream::stdoutStream;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n0;
    ofstream f1("1.dat", ios::binary);
    n1.serialize(f1);
    f1.close();
    ifstream f2("1.dat", ios::binary);
    Function::FunctionNode *root =
        Function::factory_FunctionNode(AssistFunc::binaryOut<Function::Type_FunctionNode>(f2));
    root->deserialize(f2);
    f2.close();
    (*root)();
    system("pause");
    delete root;
    return 0;
}