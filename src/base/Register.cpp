#include "Register.h"
#include "Stream.h"

namespace Register {

std::map<ID_ObjectStd, void *> dict_ID2ObjectStd{{ID_ObjectStd::Key_stdin, &Stream::stdinStream},
                                                  {ID_ObjectStd::Key_stdout, &Stream::stdoutStream},
                                                  {ID_ObjectStd::Key_stderr, &Stream::stderrStream}};

}; // namespace Register