#include "tff.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;

using TP_FuncNode = Function::Type_FunctionNode;
using TP_Stream = Stream::Type_Stream;
using ID = Register::ID_ObjectStd;

// 结点反射
map<string, TP_FuncNode> dict_Token2Node{
    {"0", TP_FuncNode::Type_ThreadNode},   {"1", TP_FuncNode::Type_AtomNode},    {"3", TP_FuncNode::Type_AssignNode},
    {"4", TP_FuncNode::Type_ExecuteNode},  {"5", TP_FuncNode::Type_DefineNode},  {"6", TP_FuncNode::Type_ConditionNode},
    {"7", TP_FuncNode::Type_StreamIONode}, {"8", TP_FuncNode::Type_ReleaseNode}, {"9", TP_FuncNode::Type_MoveNode}};

// ID反射
map<string, ID> dict_Token2ID{{"stdin", ID::Key_stdin}, {"stdout", ID::Key_stdout}, {"stderr", ID::Key_stderr}};

using TokenPtr = vector<string>::iterator;
using StdFunc = Function::StdFunctionNode::Func;
auto factory = Function::factory_FunctionNode;

// 标准对象转义处理
void *token2ObjectStd(TokenPtr &tkCur, TokenPtr &vecEnd) {
    if (*tkCur == "(")
        tkCur++;
    auto ret = Register::dict_ID2ObjectStd[dict_Token2ID[*tkCur]];
    tkCur += 2;
    return ret;
}

// 小括号内部提取
string tokenInBracket(TokenPtr &tkCur, TokenPtr &vecEnd) {
    if (*tkCur == "(")
        tkCur++;
    auto ret = *tkCur;
    tkCur += 2;
    return ret;
}

// 前置大括号处理
void startCreateNode(TokenPtr &treeStart, size_t &count, TokenPtr &vecEnd) {
    while (treeStart != vecEnd) {
        if (*treeStart == "{") {
            count++;
            treeStart++;
            break;
        }
        treeStart++;
    }
}

// 后置大括号处理
void endCreateNode(TokenPtr &treeStart, size_t &count, TokenPtr &vecEnd) {
    while (treeStart != vecEnd) {
        if (*treeStart == "}") {
            count--;
            treeStart++;
            break;
        }
        treeStart++;
    }
}

// 构造代码树
tff::FuncTree createFuncTree(TokenPtr &treeStart, TokenPtr &vecEnd) {
    size_t count = 1;
    auto count_f = [&](string &_tk) { // 计数处理
        if (_tk == "{")
            count++;
        else if (_tk == "}")
            count--;
    };
    tff::FuncTree root = nullptr;
    auto *ptr = root, *slow = root;
    auto ptr_f = [&](string &_tk) { // 结点指针控制
        if (!root) {
            root = factory(move(dict_Token2Node[*treeStart]));
            slow = root;
            ptr = root;
        } else {
            ptr = factory(move(dict_Token2Node[_tk]));
            slow->next = ptr;
            slow = slow->next;
        }
    };

    // 解析函数
    auto bigFuncParse = [&](tff::FuncTree &func) { // 单结点成员解析
        if (*treeStart == "{") {
            treeStart++;
            func = createFuncTree(treeStart, vecEnd);
        } else if (*treeStart == "(") {
            auto _func = new Function::StdFunctionNode;
            _func->func = (StdFunc)token2ObjectStd(treeStart, vecEnd);
            _func->next = new Function::EndNode;
            func = _func;
        } else {
            count--;
            func = createFuncTree(treeStart, vecEnd);
        }
    };
    auto funcParse = [&](tff::FuncTree &func) { // 一般结点成员解析
        if (*treeStart == "{") {
            treeStart++;
            func = createFuncTree(treeStart, vecEnd);
        } else if (*treeStart == "(") {
            auto _func = new Function::StdFunctionNode;
            _func->func = (StdFunc)token2ObjectStd(treeStart, vecEnd);
            _func->next = new Function::EndNode;
            func = _func;
        }
    };
    auto exprParse = [&](tff::TryteExpr &expr) { // 表达式成员解析
        if (*treeStart == "(")
            expr = stoi(tokenInBracket(treeStart, vecEnd));
        else {
            expr = (*treeStart).c_str();
            treeStart++;
        }
    };
    auto ostrParse = [&](Stream::Ostream *&ostr) { // 输出流成员解析
        if (*treeStart == "(")
            ostr = (Stream::IstreamStd *)token2ObjectStd(treeStart, vecEnd);
        else {
            ostr = new Stream::StackStream((*treeStart).c_str());
            treeStart++;
        }
    };
    auto istrParse = [&](Stream::Istream *&istr) { // 输入流成员解析
        if (*treeStart == "(")
            istr = (Stream::OstreamStd *)token2ObjectStd(treeStart, vecEnd);
        else {
            istr = new Stream::StackStream((*treeStart).c_str());
            treeStart++;
        }
    };

    while (treeStart != vecEnd) {
        auto &tk = *treeStart;
        // 结点处理
        if (tk == "0") { // ThreadNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::ThreadNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            bigFuncParse(cur->thread);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "1") { // AtomNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::AtomNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            bigFuncParse(cur->atom);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "3") { // AssignNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::AssignNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            exprParse(cur->dest);
            exprParse(cur->value);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "4") { // ExecuteNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::ExecuteNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            exprParse(cur->addr);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "5") { // DefineNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::DefineNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            exprParse(cur->addr);
            funcParse(cur->func);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "6") { // ConditionNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::ConditionNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            exprParse(cur->cond);
            funcParse(cur->func_T);
            // treeStart++;
            funcParse(cur->func_F);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "7") { // StreamIONode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::StreamIONode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            ostrParse(cur->str_O);
            istrParse(cur->str_I);
            exprParse(cur->size);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "8") { // ReleaseNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::ReleaseNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            exprParse(cur->addr);
            endCreateNode(treeStart, count, vecEnd);
        } else if (tk == "9") { // MoveNode
            ptr_f(tk);
            auto *cur = dynamic_cast<Function::MoveNode *>(ptr);
            startCreateNode(treeStart, count, vecEnd);
            exprParse(cur->dest);
            exprParse(cur->addr);
            endCreateNode(treeStart, count, vecEnd);
        }
        count_f(*treeStart);
        if (count == 0) {
            treeStart++;
            break;
        }
    }
    slow->next = new Function::EndNode;
    return root;
}

// 编译器主函数
void compiler(vector<string> &&tokens, const char *tffb_file) {
    if (tokens.empty())
        return;
    tokens.emplace_back(string("}"));
    auto &&treeStart = tokens.begin(), &&vecEnd = tokens.end();
    auto &&root = createFuncTree(treeStart, vecEnd);
    ofstream compiled_code(tffb_file, ios::binary);
    root->serialize(compiled_code);
    compiled_code.close();
}