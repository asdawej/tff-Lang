#include "std/Register.h"
#include "std/StdFunction.h"
#include "std/StdStream.h"

namespace Register {

std::map<ID_ObjectStd, void *> dict_ID2ObjectStd{
    {ID_ObjectStd::Key_stdin, &tff::stdinStream},   {ID_ObjectStd::Key_stdout, &tff::stdoutStream},
    {ID_ObjectStd::Key_stderr, &tff::stderrStream}, {ID_ObjectStd::Key_add, (void *)tff::add},
    {ID_ObjectStd::Key_sub, (void *)tff::sub},      {ID_ObjectStd::Key_mul, (void *)tff::mul},
    {ID_ObjectStd::Key_div, (void *)tff::div},      {ID_ObjectStd::Key_mod, (void *)tff::mod},
    {ID_ObjectStd::Key_sleep, (void *)tff::sleep}};

std::map<std::string, TP_FuncNode> dict_Token2Node{
    {"0", TP_FuncNode::Type_ThreadNode},   {"1", TP_FuncNode::Type_AtomNode},    {"3", TP_FuncNode::Type_AssignNode},
    {"4", TP_FuncNode::Type_ExecuteNode},  {"5", TP_FuncNode::Type_DefineNode},  {"6", TP_FuncNode::Type_ConditionNode},
    {"7", TP_FuncNode::Type_StreamIONode}, {"8", TP_FuncNode::Type_ReleaseNode}, {"9", TP_FuncNode::Type_MoveNode}};

std::map<std::string, ID> dict_Token2ID{
    {"stdin", ID::Key_stdin}, {"stdout", ID::Key_stdout}, {"stderr", ID::Key_stderr},
    {"add", ID::Key_add},     {"sub", ID::Key_sub},       {"mul", ID::Key_mul},
    {"div", ID::Key_div},     {"mod", ID::Key_mod},       {"sleep", ID::Key_sleep}};

}; // namespace Register