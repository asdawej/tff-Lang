#ifndef STREAM_H
#define STREAM_H

#include "BTS.h"
#include "tff.h"
#include <iostream>

namespace Stream {

enum struct Type_Stream { Type_StackStream, Type_IstreamStd, Type_OstreamStd };

// 输入流接口
struct Istream {
    virtual Type_Stream type() = 0;
    virtual void in(BTS::Tryte) = 0;
};

// 输出流接口
struct Ostream {
    virtual Type_Stream type() = 0;
    virtual BTS::Tryte out() = 0;
};

// 内存栈数据流
struct StackStream : public Istream, public Ostream {
    StackStream() : stackAddr(0), size(0){};
    StackStream(const tff::TryteExpr &_stackAddr) : stackAddr(_stackAddr), size(0){};
    ~StackStream() = default;
    tff::TryteExpr stackAddr; // 内存栈地址
    size_t size;              // 已导通三进制字节数

    Type_Stream type() override;
    void in(BTS::Tryte) override;
    BTS::Tryte out() override;
};

// istream包装
struct IstreamStd : public Ostream {
    IstreamStd() : istr(nullptr), func_in(nullptr){};
    IstreamStd(std::istream &_istr) : istr(&_istr){};
    IstreamStd(std::istream &_istr, BTS::Tryte (*_func_in)(std::istream &)) : istr(&_istr), func_in(_func_in){};
    ~IstreamStd() = default;
    std::istream *istr;                    // 输入流
    BTS::Tryte (*func_in)(std::istream &); // 输入处理

    Type_Stream type() override;
    BTS::Tryte out() override;
};

// ostream包装
struct OstreamStd : public Istream {
    OstreamStd() : ostr(nullptr), func_out(nullptr){};
    OstreamStd(std::ostream &_ostr) : ostr(&_ostr){};
    OstreamStd(std::ostream &_ostr, void (*_func_out)(BTS::Tryte, std::ostream &))
        : ostr(&_ostr), func_out(_func_out){};
    ~OstreamStd() = default;
    std::ostream *ostr;                           // 输出流
    void (*func_out)(BTS::Tryte, std::ostream &); // 输出处理

    Type_Stream type() override;
    void in(BTS::Tryte) override;
};

extern IstreamStd stdinStream;  // stdin包装
extern OstreamStd stdoutStream; // stdout包装
extern OstreamStd stderrStream; // stderr包装

}; // namespace Stream

#endif