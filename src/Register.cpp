#include "Register.h"
#include "Stream.h"

namespace Register {

enum struct ID_ObjectStd { Key_stdin, Key_stdout, Key_stderr };

std::map<ID_ObjectStd, void *> dict_Key2ObjectStd{{ID_ObjectStd::Key_stdin, &Stream::stdinStream},
                                                   {ID_ObjectStd::Key_stdout, &Stream::stdoutStream},
                                                   {ID_ObjectStd::Key_stderr, &Stream::stderrStream}};

}; // namespace Register