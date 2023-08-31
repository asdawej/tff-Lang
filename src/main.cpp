#include "tff.h"
#include <iostream>
using namespace std;

int main() {
    Function::StreamIONode n1, n2;
    Function::EndNode n3;
    n1.size = 5;
    n1.str_O = (Stream::IstreamStd *)Register::dict_Key2ObjectStd[Register::ID_ObjectStd::Key_stdin];
    n1.str_I = new Stream::StackStream(0);
    n2.size = 5;
    n2.str_O = new Stream::StackStream(0);
    n2.str_I = (Stream::OstreamStd *)Register::dict_Key2ObjectStd[Register::ID_ObjectStd::Key_stdout];
    n1.next = &n2;
    n2.next = &n3;
    Function::FunctionNode *ptr = &n1;
    while (ptr) {
        (*ptr)();
        ptr = ptr->next;
    }
    system("pause");
    return 0;
}