#include "memory.h"


static const char tff_CACHEFILE[] = "__tff_cache__.tffb";


const char tff_FUNCTAB[] = {
    'T',// True
    'N',// Null
    'F',// False
    '0',// Left
    '1',// Right
    '2',// Locate
    '3',// Assign
    '4',// Run
    '5',// Link
    '6',// Assert
    '7',// Input (test)
    '8' // Output (test)
}; const int tff_FUNCTAB_size = 12;


int main(int argc, char* argv[]){
    if (argc != 4) return -1;
    static FILE* rfp = NULL;
    if (!strcmp(argv[1], "-r")) rfp = FOPN(argv[2], "r");
    else return -1;
    static FILE* wfp = NULL;
    wfp = FOPN(tff_CACHEFILE, "w"); FCLS(wfp);// Delete cache
    wfp = FOPN(tff_CACHEFILE, "a+");
    char c;
    while ((c = fgetc(rfp)) != EOF){
        for (RGT i = 0; i < tff_FUNCTAB_size; i++){
            if (c == tff_FUNCTAB[i]){
                if (fputc(c, wfp) != EOF) break;
                else return -1;
            }
        }
    }
    FCLS(rfp); FCLS(wfp);
    /*  TEST  */
    wfp = FOPN(tff_CACHEFILE, "r");
    char s[100];
    FSCF(wfp, "%s", s);
    PRF("%s", s);
    FCLS(wfp);
    return 0;
}
