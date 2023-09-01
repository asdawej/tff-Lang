#include "tff.h"
#include <fstream>
using namespace std;

// 解释器主函数
void interpreter(const char *tffb_file) {
    ifstream compiled_code(tffb_file, ios::binary);
    if (!compiled_code)
        return;
    auto *root = Function::factory_FunctionNode(AssistFunc::binaryOut<Function::Type_FunctionNode>(compiled_code));
    root->deserialize(compiled_code);
    (*root)();
    compiled_code.close();
}