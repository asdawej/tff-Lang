#include "tff.h"
#include <fstream>
using namespace std;

// 解释器主函数
void interpreter(const char *tffb_file) {
    ifstream compiled_code(tffb_file, ios::binary);
    compiled_code.close();
}