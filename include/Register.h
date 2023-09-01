#ifndef REGISTER_H
#define REGISTER_H

#include <functional>
#include <map>

namespace Register {

enum struct ID_ObjectStd {
    Key_stdin,  // Stream::IstreamStd*
    Key_stdout, // Stream::OstreamStd*
    Key_stderr, // Stream::OstreamStd*
};

extern std::map<ID_ObjectStd, void *> dict_Key2ObjectStd;

}; // namespace Register

#endif