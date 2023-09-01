#include "tff.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const char noteStart = '#'; // 注释起始符

// 判断是否是空白字符
bool isWhite(const char c) {
    switch (c) {
    default:
        return false;
    case '\0':
    case ' ':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
        return true;
    }
}

// 读取下一个非空白、非注释字符
char readNext(ifstream &istr) {
    char c;
    bool flag = false;
    while (!istr.eof()) {
        istr >> noskipws >> c;
        if (flag) {
            if (c == '\n')
                flag = false;
            continue;
        }
        if (c == noteStart) {
            flag = true;
            continue;
        }
        if (isWhite(c))
            continue;
        return c;
    }
}

// 非空白、非注释读取至特定字符(不包含)
string readUntilChar(ifstream &istr, const char index) {
    string s;
    char c;
    while (!istr.eof()) {
        c = readNext(istr);
        if (c == index)
            break;
        s += c;
    }
    return s;
}

using StringWithIndex = struct {
    string str;
    char index;
};

// 非空白、非注释读取至若干特定字符(返回该字符)
StringWithIndex readUntilChar(ifstream &istr, const vector<char> &indexs) {
    string s;
    char c;
    while (!istr.eof()) {
        c = readNext(istr);
        for (const auto &it : indexs) {
            if (c == it) {
                return StringWithIndex{s, c};
            }
        }
        s += c;
    }
    return StringWithIndex{s, '\0'};
}

// 分析器主函数
vector<string> analyzer(const char *tffl_file) {
    ifstream text_code(tffl_file);
    if (!text_code)
        throw(-1);
    vector<string> tokens;
    auto append = [&](string &_str) { tokens.emplace_back(_str); };            // tokens插入string
    auto appends = [&](const char *_s) { tokens.emplace_back(string(_s)); };   // tokens插入字符串
    auto appendc = [&](const char _c) { tokens.emplace_back(string(1, _c)); }; // tokens插入单字符
    auto rindexs = vector<char>{')', '}', ','};                                // 右侧标志
    char c;
    while (!text_code.eof()) {
        c = readNext(text_code);
        switch (c) {
        case '{':
        case '(':
        case '}':
        case ')':
        case ',':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            appendc(c);
        } break;
        default: {
            string str(1, c);
            auto &&strWithIndex = readUntilChar(text_code, rindexs);
            str += strWithIndex.str;
            append(str);
            appendc(strWithIndex.index);
        } break;
        };
    }
    text_code.close();
    return tokens;
}