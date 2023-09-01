#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> analyzer(const char *tffl_file);
void compiler(std::vector<std::string> tokens, const char *tffb_file);
void interpreter(const char *tffb_file);

int main(int argc, const char *argv[]) {
    char *tffl_file;
    char *tffb_file;
    if (argc > 2) {
        if (!strcasecmp("-c", argv[1])) {
            if (argc == 3) {
                auto len = strlen(argv[2]);
                tffl_file = new char[len + 1];
                strcpy(tffl_file, argv[2]);
                tffb_file = new char[len + 1];
                strcpy(tffb_file, argv[2]);
                tffb_file[len - 1] = 'b';
            } else if (argc >= 4) {
                auto len1 = strlen(argv[2]);
                auto len2 = strlen(argv[3]);
                tffl_file = new char[len1 + 1];
                strcpy(tffl_file, argv[2]);
                tffb_file = new char[len2 + 1];
                strcpy(tffb_file, argv[3]);
            }
            compiler(analyzer(tffl_file), tffb_file);
            delete tffl_file, tffb_file;
        } else if (!strcasecmp("-r", argv[1])) {
            auto len = strlen(argv[2]);
            tffb_file = new char[len + 1];
            strcpy(tffb_file, argv[2]);
            interpreter(tffb_file);
        } else if (!strcasecmp("-cr", argv[1])) {
            if (argc == 3) {
                auto len = strlen(argv[2]);
                tffl_file = new char[len + 1];
                strcpy(tffl_file, argv[2]);
                tffb_file = new char[len + 1];
                strcpy(tffb_file, argv[2]);
                tffb_file[len - 1] = 'b';
            } else if (argc >= 4) {
                auto len1 = strlen(argv[2]);
                auto len2 = strlen(argv[3]);
                tffl_file = new char[len1 + 1];
                strcpy(tffl_file, argv[2]);
                tffb_file = new char[len2 + 1];
                strcpy(tffb_file, argv[3]);
            }
            compiler(analyzer(tffl_file), tffb_file);
            interpreter(tffb_file);
            delete tffl_file, tffb_file;
        } else
            goto L_ARGWRONG;
    } else
        goto L_ARGFEW;

L_RETURN:
    return 0;
L_ARGWRONG:
    cout << "Arguments wrong!" << endl;
    return 0;
L_ARGFEW:
    cout << "More arguments needed!" << endl;
    return 0;
}