#include "Function.h"
#include "AssistFunc.h"
#include "Register.h"

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
    default:
        break;
    }
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

void AssignNode::operator()() { tff::stack[dest] = value; }

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
    FunctionNode::connect(tff::funcTable[addr], next);
    next = tff::funcTable[addr];
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

void DefineNode::operator()() { tff::funcTable[addr] = func; }

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
    if (cond > 0) {
        FunctionNode::connect(func_T, next);
        next = func_T;
    } else if (cond < 0) {
        FunctionNode::connect(func_F, next);
        next = func_F;
    }
}

// === StreamIONode ===

Type_FunctionNode StreamIONode::type() { return Type_FunctionNode::Type_StreamIONode; }

void StreamIONode::serialize(std::ostream &ostr) {
    AssistFunc::binaryIn(ostr, type());
    if (str_O->type() == Stream::Type_Stream::Type_StackStream) {
        AssistFunc::binaryIn(ostr, str_O->type());
        auto &_str_O_stack = *(Stream::StackStream *)str_O;
        AssistFunc::binaryIn(ostr, BTS::Tryte(_str_O_stack.stackAddr - tff::stack));
    } else {
        AssistFunc::binaryIn(ostr, str_O->type());
        auto &_str_O_IStd = *(Stream::IstreamStd *)str_I;
        for (auto &_pr : Register::dict_Key2ObjectStd) {
            if (_pr.second == _str_O_IStd.istr) {
                AssistFunc::binaryIn(ostr, _pr.first);
                break;
            }
        }
    }
    if (str_I->type() == Stream::Type_Stream::Type_StackStream) {
        AssistFunc::binaryIn(ostr, str_I->type());
        auto &_str_I_stack = *(Stream::StackStream *)str_I;
        AssistFunc::binaryIn(ostr, BTS::Tryte(_str_I_stack.stackAddr - tff::stack));
    } else {
        AssistFunc::binaryIn(ostr, str_I->type());
        auto &_str_I_OStd = *(Stream::OstreamStd *)str_I;
        for (auto &_pr : Register::dict_Key2ObjectStd) {
            if (_pr.second == _str_I_OStd.ostr) {
                AssistFunc::binaryIn(ostr, _pr.first);
                break;
            }
        }
    }
    AssistFunc::binaryIn(ostr, size);
    next->serialize(ostr);
}

void StreamIONode::deserialize(std::istream &istr) {
    if (AssistFunc::binaryOut<Stream::Type_Stream>(istr) == Stream::Type_Stream::Type_StackStream)
        str_O = new Stream::StackStream(tff::stack[AssistFunc::binaryOut<BTS::Tryte>(istr)]);
    else
        str_O = (Stream::Ostream *)Register::dict_Key2ObjectStd[AssistFunc::binaryOut<Register::ID_ObjectStd>(istr)];
    if (AssistFunc::binaryOut<Stream::Type_Stream>(istr) == Stream::Type_Stream::Type_StackStream)
        str_I = new Stream::StackStream(tff::stack[AssistFunc::binaryOut<BTS::Tryte>(istr)]);
    else
        str_I = (Stream::Istream *)Register::dict_Key2ObjectStd[AssistFunc::binaryOut<Register::ID_ObjectStd>(istr)];
    size = AssistFunc::binaryOut<tff::TryteExpr>(istr);
    next = factory_FunctionNode(AssistFunc::binaryOut<Type_FunctionNode>(istr));
    next->deserialize(istr);
}

void StreamIONode::operator()() { str_I->in(str_O->out()); }

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

void ReleaseNode::operator()() { tff::funcTable[addr] = nullptr; }

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

void MoveNode::operator()() { tff::funcTable[dest] = tff::funcTable[addr]; }

// === EndNode ===

Type_FunctionNode EndNode::type() { return Type_FunctionNode::Type_EndNode; }

void EndNode::serialize(std::ostream &ostr) { AssistFunc::binaryIn(ostr, type()); }

void EndNode::deserialize(std::istream &istr) {}

void EndNode::operator()() {}

}; // namespace Function