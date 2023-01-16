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


/*  tff interpreter main  */
int main(int argc, char* argv[]){
    if (!tff_INIT()) return -1;
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

    /*  test  */

    wfp = FOPN(tff_CACHEFILE, "r");
    char s[100];
    FSCF(wfp, "%s", s);
    PRF("%s", s);
    FCLS(wfp);
    return 0;
}


/*  internal test main  */
int main1(){
    tff_INIT();
    VU loc = {
        .f_val = "T",
        .l_val = "TN"
    };
    VU val = {
        .f_val = "F",
        .l_val = "FN"
    };
    tff_assign(val, loc);
    PRF("%s\t%s\n", tff_MEMORY.MEM[0].AREA[0].f_val, tff_MEMORY.MEM[0].AREA[0].l_val);
    PRF("%s\t%s\n", tff_MEMORY.MEM[0].name_AREA, tff_MEMORY.MEM[0].tab_AREA[0]);
    PRF("%s\t%s\n", tff_MEMORY.MEM[1].AREA[0].f_val, tff_MEMORY.MEM[1].AREA[0].l_val);
    PRF("%s\t%s\n", tff_MEMORY.MEM[1].name_AREA, tff_MEMORY.MEM[1].tab_AREA[0]);
    return 0;
}