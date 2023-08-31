#ifndef FUNCTION_H
#define FUNCTION_H

#include "BTS.h"
#include "Stream.h"
#include "TryteExpr.h"

namespace Function {

enum struct Type_FunctionNode {
    Type_ThreadNode = 0,
    Type_AtomNode = 1,
    Type_AssignNode = 3,
    Type_ExecuteNode = 4,
    Type_DefineNode = 5,
    Type_ConditionNode = 6,
    Type_StreamIONode = 7,
    Type_ReleaseNode = 8,
    Type_MoveNode = 9,
    Type_EndNode,
    Type_StdFunctionNode
};

// 代码树结点基类
struct FunctionNode {
    FunctionNode() : next(nullptr){};
    virtual ~FunctionNode() = default;
    FunctionNode *next; // 后继节点

    using NodePair = std::pair<FunctionNode *, FunctionNode *>;
    // 返回终止结点及其前一个结点
    static NodePair findEndNode(FunctionNode *);
    // 连接两个代码树
    static void connect(FunctionNode *, FunctionNode *);
    // 对象类型反射
    virtual Type_FunctionNode type() = 0;
    // 代码运行接口
    virtual void operator()() = 0;
    // 二进制序列化
    virtual void serialize(std::ostream &) = 0;
    // 二进制反序列化
    virtual void deserialize(std::istream &) = 0;
};

// `0`结点, 将一个代码树设置为子线程
struct ThreadNode : public FunctionNode {
    ThreadNode() : thread(nullptr){};
    ~ThreadNode() { delete thread; };
    FunctionNode *thread; // 子线程代码树

    Type_FunctionNode type() override;
    void operator()() override; // # 暂时无法实现
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `1`结点, 将一个代码树设置为原子操作
struct AtomNode : public FunctionNode {
    AtomNode() : atom(nullptr){};
    ~AtomNode() { delete atom; };
    FunctionNode *atom; // 原子操作代码树

    Type_FunctionNode type() override;
    void operator()() override; // # 暂时无法实现
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `3`结点, 赋值操作
struct AssignNode : public FunctionNode {
    AssignNode() : dest(0), value(0){};
    ~AssignNode() = default;
    tff::TryteExpr dest, value; // 把value赋给dest地址

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `4`结点, 执行委托地址上的代码树
struct ExecuteNode : public FunctionNode {
    ExecuteNode() : addr(0){};
    ~ExecuteNode() = default;
    tff::TryteExpr addr; // 委托地址

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `5`结点, 将代码树委托给一个地址
struct DefineNode : public FunctionNode {
    DefineNode() : addr(0), func(nullptr){};
    ~DefineNode() { delete func; };
    tff::TryteExpr addr; // 委托地址
    FunctionNode *func;  // 代码树

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `6`结点, 条件转移
struct ConditionNode : public FunctionNode {
    ConditionNode() : cond(0), func_T(nullptr), func_F(nullptr){};
    ~ConditionNode() { delete func_T, func_F; };
    tff::TryteExpr cond;           // 条件
    FunctionNode *func_T, *func_F; // 正值代码树func_T，负值代码树func_F

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `7`结点, 导通两个流
struct StreamIONode : public FunctionNode {
    StreamIONode() : str_I(nullptr), str_O(nullptr), size(0){};
    ~StreamIONode() = default;
    Stream::Ostream *str_O; // 输出流
    Stream::Istream *str_I; // 输入流
    tff::TryteExpr size;    // 三进制字节数

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `8`结点, 解除代码树委托
struct ReleaseNode : public FunctionNode {
    ReleaseNode() : addr(0){};
    ~ReleaseNode() = default;
    tff::TryteExpr addr; // 委托地址

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// `9`结点, 传播代码树委托
struct MoveNode : public FunctionNode {
    MoveNode() : dest(0), addr(0){};
    ~MoveNode() = default;
    tff::TryteExpr dest; // 传播地址
    tff::TryteExpr addr; // 委托地址

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// 终止结点, 释放运行终止信号
struct EndNode : public FunctionNode {
    EndNode() = default;
    ~EndNode() = default;

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

// 库函数包装结点, 运行相应的函数
struct StdFunctionNode : public FunctionNode {
    using Func = void (*)();
    StdFunctionNode() : func(nullptr){};
    ~StdFunctionNode() = default;
    Func func;

    Type_FunctionNode type() override;
    void operator()() override;
    void serialize(std::ostream &) override;
    void deserialize(std::istream &) override;
};

FunctionNode *factory_FunctionNode(Type_FunctionNode &&);

}; // namespace Function

#endif