#include "../include/tff.h"
#include <iostream>
using namespace std;

int main() {
    BTS::Tryte t(27);
    cout << (int)t << t.repr();
    system("pause");
    return 0;
}