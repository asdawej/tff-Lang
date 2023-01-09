#include "memory.h"


const char tff_CONFIG_DEFAULT[] = "config.tffc";


int main(int argc, char* argv[]){
    if (!strcmp(argv[1], "-r")){
        FILE* main_code = FOPN(argv[2], "r");
        FILE* config;
        if (!strcmp(argv[3], "default")) config = FOPN(tff_CONFIG_DEFAULT, "r");
        else config = FOPN(argv[3], "r");
        char c;
        while ((c = fgetc(main_code)) != EOF){}
    }
}
