#include "Function.h"

namespace Function {

FunctionNode::NodePair FunctionNode::findEndNode(FunctionNode *start) {
    auto *ptr = start;
    while (ptr->next != nullptr) {
        if (ptr->next->type() == Type_FunctionNode::Type_EndNode)
            return FunctionNode::NodePair{ptr, ptr->next};
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

Type_FunctionNode ThreadNode::type() { return Type_FunctionNode::Type_ThreadNode; }

Type_FunctionNode AtomNode::type() { return Type_FunctionNode::Type_AtomNode; }

Type_FunctionNode AssignNode::type() { return Type_FunctionNode::Type_AssignNode; }

Type_FunctionNode ExecuteNode::type() { return Type_FunctionNode::Type_ExecuteNode; }

Type_FunctionNode DefineNode::type() { return Type_FunctionNode::Type_DefineNode; }

Type_FunctionNode ConditionNode::type() { return Type_FunctionNode::Type_ConditionNode; }

Type_FunctionNode StreamIONode::type() { return Type_FunctionNode::Type_StreamIONode; }

Type_FunctionNode ReleaseNode::type() { return Type_FunctionNode::Type_ReleaseNode; }

Type_FunctionNode MoveNode::type() { return Type_FunctionNode::Type_MoveNode; }

Type_FunctionNode EndNode::type() { return Type_FunctionNode::Type_EndNode; }

}; // namespace Function