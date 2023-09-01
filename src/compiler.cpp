#include "tff.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// 编译器主函数
void compiler(vector<string> tokens, const char *tffb_file) {
    ofstream compiled_code(tffb_file, ios::binary);
    compiled_code.close();
}