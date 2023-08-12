#ifndef STREAM_H
#define STREAM_H

#include "BTS.h"
#include <iostream>

namespace Stream {

// 数据流基类
struct Stream {
    Stream() = default;
    virtual ~Stream() = default;
    virtual void in(BTS::Tryte) = 0;
    virtual BTS::Tryte out(void) = 0;
};

// 内存栈数据流
struct StackStream : public Stream {
    StackStream();
    ~StackStream();
    BTS::Tryte *stackAddr; // 内存栈地址
    void in(BTS::Tryte);
    BTS::Tryte out(void);
};

// istream包装
struct IstreamStream : public Stream {
    IstreamStream();
    ~IstreamStream();
    std::istream *istr; // 输入流
    void in(BTS::Tryte);
    BTS::Tryte out(void);
};

// ostream包装
struct OstreamStream : public Stream {
    OstreamStream();
    ~OstreamStream();
    std::ostream *ostr; // 输出流
    void in(BTS::Tryte);
    BTS::Tryte out(void);
};

}; // namespace Stream

#endif