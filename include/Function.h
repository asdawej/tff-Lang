#ifndef FUNCTION_H
#define FUNCTION_H

#include "BTS.h"
#include "Stream.h"
#include "tff.h"

namespace Function {

// 代码树结点基类
struct FunctionNode {
    FunctionNode() : next(nullptr){};
    virtual ~FunctionNode() = 0;
    FunctionNode *next;            // 后继节点
    virtual void operator()() = 0; // 代码运行接口
};

// `0`结点, 将一个代码树设置为子线程
struct ThreadNode : public FunctionNode {
    ThreadNode() : thread(nullptr){};
    ~ThreadNode() { delete thread; };
    FunctionNode *thread;           // 子线程代码树
    void operator()() override = 0; // # 暂时无法实现
};

// `1`结点, 将一个代码树设置为原子操作
struct AtomNode : public FunctionNode {
    AtomNode() : atom(nullptr){};
    ~AtomNode() { delete atom; };
    FunctionNode *atom;             // 原子操作代码树
    void operator()() override = 0; // # 暂时无法实现
};

// `3`结点, 赋值操作
struct AssignNode : public FunctionNode {
    AssignNode() : dest(0), value(0){};
    ~AssignNode() = default;
    BTS::Tryte dest, value; // 把value赋给dest地址
    void operator()() override;
};

// `4`结点, 执行委托地址上的代码树
struct ExecuteNode : public FunctionNode {
    ExecuteNode() : addr(0){};
    ~ExecuteNode() = default;
    BTS::Tryte addr; // 委托地址
    void operator()() override;
};

// `5`结点, 将代码树委托给一个地址
struct DefineNode : public FunctionNode {
    DefineNode() : addr(0), func(nullptr){};
    ~DefineNode() { delete func; };
    BTS::Tryte addr;    // 委托地址
    FunctionNode *func; // 代码树
    void operator()() override;
};

// `6`结点, 条件转移
struct ConditionNode : public FunctionNode {
    ConditionNode() : cond(0), func_T(nullptr), func_F(nullptr){};
    ~ConditionNode() { delete func_T, func_F; };
    BTS::Tryte cond;               // 条件
    FunctionNode *func_T, *func_F; // 正值代码树func_T，负值代码树func_F
    void operator()() override;
};

// `7`结点, 导通两个流
struct StreamIONode : public FunctionNode {
    StreamIONode() : str_I(nullptr), str_O(nullptr), size(0){};
    ~StreamIONode() = default; // # 需要判断是否需要delete
    Stream::Istream *str_I;    // 输入流
    Stream::Ostream *str_O;    // 输出流
    BTS::Tryte size;           // 三进制字节数
    void operator()() override;
};

// `8`结点, 解除代码树委托
struct ReleaseNode : public FunctionNode {
    ReleaseNode() : addr(0){};
    ~ReleaseNode() = default;
    BTS::Tryte addr; // 委托地址
    void operator()() override;
};

// `9`结点, 传播代码树委托
struct MoveNode : public FunctionNode {
    MoveNode() : dest(0), addr(0){};
    ~MoveNode() = default;
    BTS::Tryte dest; // 传播地址
    BTS::Tryte addr; // 委托地址
    void operator()() override;
};

// 终止结点, 释放运行终止信号
struct EndNode : public FunctionNode {
    EndNode() = default;
    ~EndNode() = default;
    void operator()() override;
};

}; // namespace Function

#endif