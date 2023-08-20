#include "../include/tff.h"
#include <iostream>
using namespace std;

int main() {
    Stream::StackStream s(tff::stack[0]);
    for (auto i = 0; i < 5; i++)
        s.in(Stream::stdinStream.out());
    Stream::StackStream s1(tff::stack[0]);
    for (auto i = 0; i < 5; i++)
        Stream::stdoutStream.in(s1.out());
    system("pause");
    return 0;
}