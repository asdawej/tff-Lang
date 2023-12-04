#include "Function.h"
#include "AssistFunc.h"
#include "std/Register.h"
#include "tff.h"

#ifdef __MINGW32__ // https://github.com/meganz/mingw-std-threads
#include <mingw.mutex.h>
#include <mingw.thread.h>
#else
#include <mutex>
#include <thread>
#endif

namespace Function {

FunctionNode *factory_FunctionNode(Type_FunctionNode &&_tp) {
    using TP = Type_FunctionNode;
    switch (_tp) {
    case TP::Type_ThreadNode:
        return new ThreadNode;
        break;
    case TP::Type_AtomNode:
        return new AtomNode;
        break;
    case TP::Type_AssignNode:
        return new AssignNode;
        break;
    case TP::Type_ExecuteNode:
        return new ExecuteNode;
        break;
    case TP::Type_DefineNode:
        return new DefineNode;
        break;
    case TP::Type_ConditionNode:
        return new ConditionNode;
        break;
    case TP::Type_StreamIONode:
        return new StreamIONode;
        break;
    case TP::Type_ReleaseNode:
        return new ReleaseNode;
        break;
    case TP::Type_MoveNode:
        return new MoveNode;
        break;
    case TP::Type_EndNode:
        return new EndNode;
        break;
    case TP::Type_StdFunctionNode:
        return new StdFunctionNode;
    default:
        break;
    }
    return nullptr;
}

FunctionNode::NodePair FunctionNode::findEndNode(FunctionNode *start) {
    auto *ptr = start;
    while (ptr->next != nullptr) {
        if (ptr->next->type() == Type_FunctionNode::Type_EndNode)
            return FunctionNode::NodePair(ptr, ptr->next);
        ptr = ptr->next;
    }
    return FunctionNode::NodePair{ptr, nullptr};
}

void FunctionNode::connect(FunctionNode *f_root, FunctionNode *l_root) {
    auto &&_pr = findEndNode(f_root);
    if (_pr.second)
        delete _pr.second;
    _pr.first->next = l_root;
}

// 辅助函数, 取表达式值
BTS::Tryte _getValue(tff::TryteExpr expr) {
    auto count = expr.suffix;
    auto ret = expr.value;
    while (count > 0) {
        ret = tff::stack[ret];
        count--;
    }
    return ret;
}

// === ThreadNode ===

Type_FunctionNode ThreadNode::type() { return Type_FunctionNode::Type_ThreadNode; }

void ThreadNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    thread->serialize(ostr);
    next->serialize(ostr);
}

void ThreadNode::deserialize(std::istream &istr) {
    thread = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    thread->deserialize(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void ThreadNode::operator()() {
    if (next)
        (*next)();
}

// === AtomNode ===

Type_FunctionNode AtomNode::type() { return Type_FunctionNode::Type_AtomNode; }

void AtomNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    atom->serialize(ostr);
    next->serialize(ostr);
}

void AtomNode::deserialize(std::istream &istr) {
    atom = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    atom->deserialize(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void AtomNode::operator()() {
    if (next)
        (*next)();
}

// === AssignNode ===

Type_FunctionNode AssignNode::type() { return Type_FunctionNode::Type_AssignNode; }

void AssignNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, dest);
    AssistFunc::binaryIn(ostr, value);
    next->serialize(ostr);
}

void AssignNode::deserialize(std::istream &istr) {
    dest = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    value = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void AssignNode::operator()() {
    tff::stack[_getValue(dest)] = _getValue(value);
    if (next)
        (*next)();
}

// === ExecuteNode ===

Type_FunctionNode ExecuteNode::type() { return Type_FunctionNode::Type_ExecuteNode; }

void ExecuteNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, addr);
    next->serialize(ostr);
}

void ExecuteNode::deserialize(std::istream &istr) {
    addr = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void ExecuteNode::operator()() {
    (*tff::funcTable[_getValue(addr)])();
    if (next)
        (*next)();
}

// === DefineNode ===

Type_FunctionNode DefineNode::type() { return Type_FunctionNode::Type_DefineNode; }

void DefineNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, addr);
    func->serialize(ostr);
    next->serialize(ostr);
}

void DefineNode::deserialize(std::istream &istr) {
    addr = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    func = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    func->deserialize(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void DefineNode::operator()() {
    tff::funcTable[_getValue(addr)] = func;
    if (next)
        (*next)();
}

// === ConditionNode ===

Type_FunctionNode ConditionNode::type() { return Type_FunctionNode::Type_ConditionNode; }

void ConditionNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, cond);
    func_T->serialize(ostr);
    func_F->serialize(ostr);
    next->serialize(ostr);
}

void ConditionNode::deserialize(std::istream &istr) {
    cond = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    func_T = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    func_T->deserialize(istr);
    func_F = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    func_F->deserialize(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void ConditionNode::operator()() {
    auto &&_cond = _getValue(cond);
    if (_cond > 0)
        (*func_T)();
    else if (_cond < 0)
        (*func_F)();
    if (next)
        (*next)();
}

// === StreamIONode ===

StreamIONode::~StreamIONode() {
    if (str_O->type() == Stream::Type_Stream::Type_StackStream)
        delete str_O;
    if (str_I->type() == Stream::Type_Stream::Type_StackStream)
        delete str_I;
}

Type_FunctionNode StreamIONode::type() { return Type_FunctionNode::Type_StreamIONode; }

void StreamIONode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, str_O->type());
    if (str_O->type() == Stream::Type_Stream::Type_StackStream) { // 栈输出
        auto &_str_O_stack = *dynamic_cast<Stream::StackStream *>(str_O);
        AssistFunc::binaryIn(ostr, _str_O_stack.stackAddr);
    } else { // 标准输入流输出
        auto *_str_O_IStd = dynamic_cast<Stream::IstreamStd *>(str_O);
        for (auto &_pr : Register::dict_ID2ObjectStd) {
            if (_pr.second == _str_O_IStd) {
                AssistFunc::binaryIn(ostr, _pr.first);
                break;
            }
        }
    }
    AssistFunc::binaryIn(ostr, str_I->type());
    if (str_I->type() == Stream::Type_Stream::Type_StackStream) { // 栈输入
        auto &_str_I_stack = *dynamic_cast<Stream::StackStream *>(str_I);
        AssistFunc::binaryIn(ostr, _str_I_stack.stackAddr);
    } else { // 标准输出流输入
        auto *_str_I_OStd = dynamic_cast<Stream::OstreamStd *>(str_I);
        for (auto &_pr : Register::dict_ID2ObjectStd) {
            if (_pr.second == _str_I_OStd) {
                AssistFunc::binaryIn(ostr, _pr.first);
                break;
            }
        }
    }
    AssistFunc::binaryIn(ostr, size);
    next->serialize(ostr);
}

void StreamIONode::deserialize(std::istream &istr) {
    if (AssistFunc::binaryOut<Stream::Type_Stream>(istr) == Stream::Type_Stream::Type_StackStream) // 栈输出
        str_O = new Stream::StackStream(AssistFunc::binaryOut<tff::TryteExpr>(istr));
    else // 标准输入流输出
        str_O = (Stream::Ostream *)Register::dict_ID2ObjectStd[AssistFunc::binaryOut<Register::ID_ObjectStd>(istr)];
    if (AssistFunc::binaryOut<Stream::Type_Stream>(istr) == Stream::Type_Stream::Type_StackStream) // 栈输入
        str_I = new Stream::StackStream(AssistFunc::binaryOut<tff::TryteExpr>(istr));
    else // 标准输出流输入
        str_I = (Stream::Istream *)Register::dict_ID2ObjectStd[AssistFunc::binaryOut<Register::ID_ObjectStd>(istr)];
    size = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void StreamIONode::operator()() {
    int count = _getValue(size);
    while (count > 0) {
        str_I->in(str_O->out());
        count--;
    }
    // 栈流恢复
    if (str_I->type() == Stream::Type_Stream::Type_StackStream)
        dynamic_cast<Stream::StackStream *>(str_I)->size = 0;
    if (str_O->type() == Stream::Type_Stream::Type_StackStream)
        dynamic_cast<Stream::StackStream *>(str_O)->size = 0;
    if (next)
        (*next)();
}

// === ReleaseNode ===

Type_FunctionNode ReleaseNode::type() { return Type_FunctionNode::Type_ReleaseNode; }

void ReleaseNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, addr);
    next->serialize(ostr);
}

void ReleaseNode::deserialize(std::istream &istr) {
    addr = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void ReleaseNode::operator()() {
    tff::funcTable[_getValue(addr)] = nullptr;
    if (next)
        (*next)();
}

// === MoveNode ===

Type_FunctionNode MoveNode::type() { return Type_FunctionNode::Type_MoveNode; }

void MoveNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    AssistFunc::binaryIn(ostr, dest);
    AssistFunc::binaryIn(ostr, addr);
    next->serialize(ostr);
}

void MoveNode::deserialize(std::istream &istr) {
    dest = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    addr = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void MoveNode::operator()() {
    tff::funcTable[_getValue(dest)] = tff::funcTable[_getValue(addr)];
    if (next)
        (*next)();
}

// === EndNode ===

Type_FunctionNode EndNode::type() { return Type_FunctionNode::Type_EndNode; }

void EndNode::serialize(std::ostream &ostr) { AssistFunc::binaryIn(ostr, type()); }

void EndNode::deserialize(std::istream &istr) {}

void EndNode::operator()() {}

// === StdFunctionNode ===

Type_FunctionNode StdFunctionNode::type() { return Type_FunctionNode::Type_StdFunctionNode; }

void StdFunctionNode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    for (auto &_pr : Register::dict_ID2ObjectStd) {
        if (_pr.second == func) {
            AssistFunc::binaryIn(ostr, _pr.first);
            break;
        }
    }
    next->serialize(ostr);
}

void StdFunctionNode::deserialize(std::istream &istr) {
    func = (Func)Register::dict_ID2ObjectStd[AssistFunc::binaryOut<Register::ID_ObjectStd>(istr)];
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void StdFunctionNode::operator()() {
    func();
    if (next)
        (*next)();
}

}; // namespace Function