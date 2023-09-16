#ifndef REGISTER_H
#define REGISTER_H

#include "Function.h"
#include <map>
#include <string>

namespace Register {

enum struct ID_ObjectStd {
    Key_stdin,  // Stream::IstreamStd*
    Key_stdout, // Stream::OstreamStd*
    Key_stderr, // Stream::OstreamStd*
    Key_add
};

extern std::map<ID_ObjectStd, void *> dict_ID2ObjectStd;

using TP_FuncNode = Function::Type_FunctionNode;
using TP_Stream = Stream::Type_Stream;
using ID = Register::ID_ObjectStd;

// 结点反射
extern std::map<std::string, TP_FuncNode> dict_Token2Node;

// ID反射
extern std::map<std::string, ID> dict_Token2ID;

}; // namespace Register

#endif