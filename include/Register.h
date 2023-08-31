#ifndef REGISTER_H
#define REGISTER_H

#include <functional>
#include <map>

namespace Register {

enum struct ID_ObjectStd { Key_stdin, Key_stdout, Key_stderr };

extern std::map<ID_ObjectStd, void *> dict_Key2ObjectStd;

}; // namespace Register

#endif