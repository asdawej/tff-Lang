#include "std/Register.h"
#include "std/StdFunction.h"
#include "std/StdStream.h"

namespace Register {

std::map<ID_ObjectStd, void *> dict_ID2ObjectStd{{ID_ObjectStd::Key_stdin, &stdinStream},
                                                 {ID_ObjectStd::Key_stdout, &stdoutStream},
                                                 {ID_ObjectStd::Key_stderr, &stderrStream},
                                                 {ID_ObjectStd::Key_add, (void *)add}};

std::map<std::string, TP_FuncNode> dict_Token2Node{
    {"0", TP_FuncNode::Type_ThreadNode},   {"1", TP_FuncNode::Type_AtomNode},    {"3", TP_FuncNode::Type_AssignNode},
    {"4", TP_FuncNode::Type_ExecuteNode},  {"5", TP_FuncNode::Type_DefineNode},  {"6", TP_FuncNode::Type_ConditionNode},
    {"7", TP_FuncNode::Type_StreamIONode}, {"8", TP_FuncNode::Type_ReleaseNode}, {"9", TP_FuncNode::Type_MoveNode}};

std::map<std::string, ID> dict_Token2ID{
    {"stdin", ID::Key_stdin}, {"stdout", ID::Key_stdout}, {"stderr", ID::Key_stderr}, {"add", ID::Key_add}};

}; // namespace Register