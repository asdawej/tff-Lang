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


// tff_PROCESS_STACK[0] and tff_BREAKPOINT_STACK[0] store lengths.
// In tff_PROCESS_STACK, there are {0, '2', '3', '4', '5', '6', '7', '8'}, and 0 is VU input flag.
// In tff_BREAKPOINT_STACK, there are the positions to back.
// tff_REGISTER is for VU input.
static char tff_PROCESS_STACK[128] = {0};
static size_t tff_BREAKPOINT_STACK[4096] = {0};
static VU tff_REGISTER[2] = {
    {
        .f_val = "N",
        .l_val = "N"
    },
    {
        .f_val = "N",
        .l_val = "N"
    }
};


// Flags to determine what to do.
static int tff_mode = 1;            // -1: Local run mode;  0: Record mode;     1: Run mode.
static int tff_flag_args = -1;      // -1: not arg;         0: The first arg;   1: The second arg.
static int tff_flag_flval = -1;     // -1: not arg;         0: The fval;        1: The lval.
static int tff_flag_assert = -2;    // -1: False assert;    0: Null assert;     1: True;                -2: Not assert.
static int tff_count_lrclose = 0;   // Used when tff_mode == 0; if it goes back to 0, tff_mode comes to -1 or 1.


// Functions Declarations
static void tff_meth_2(void);
static void tff_meth_3(void);
static void tff_meth_4(void);
static void tff_meth_5(void);
static void tff_meth_6(void);
static void tff_meth_7(void);
static void tff_meth_8(void);


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

    wfp = FOPN(tff_CACHEFILE, "r");
    char s[300];
    FSCF(wfp, "%s", s);
    PRF("%s", s);
    FCLS(wfp);
    return 0;
}


static void tff_meth_2(void){
    tff_getval(&tff_REGISTER[tff_flag_args], tff_REGISTER[tff_flag_args]);
}


static void tff_meth_3(void){
    tff_assign(tff_REGISTER[1], tff_REGISTER[0]);
}


static void tff_meth_4(void){

}


static void tff_meth_5(void){

}


static void tff_meth_6(void){

}


static void tff_meth_7(void){
    VU temp;
    char c;
    _INPUT_f:
        temp.f_val = (BTS)MLC(1); temp.f_val[0] = '\0';
        size_t _fsave = 0;
        PRF("Input fval:");
        while ((c = GCR()) != '\n'){
            if ((c != 'T') && (c != 'N') && (c != 'F')){
                free(temp.f_val);
                fflush(stdin);
                goto _INPUT_f;
            }
            temp.f_val[_fsave] = c;
            if (temp.f_val = (BTS)RLC(temp.f_val, (++_fsave)+1)){
                temp.f_val[_fsave] = '\0';
            }
            else exit(-1);
        }
    _INPUT_l:
        temp.l_val = (BTS)MLC(1); temp.l_val[0] = '\0';
        size_t _lsave = 0;
        PRF("Input lval:");
        while ((c = GCR()) != '\n'){
            if ((c != 'T') && (c != 'N') && (c != 'F')){
                free(temp.l_val);
                fflush(stdin);
                goto _INPUT_l;
            }
            temp.l_val[_lsave] = c;
            if (temp.l_val = (BTS)RLC(temp.l_val, (++_lsave)+1)){
                temp.l_val[_lsave] = '\0';
            }
            else exit(-1);
        }
    tff_assign(temp, tff_REGISTER[0]);
}


static void tff_meth_8(void){
    VU temp;
    tff_getval(&temp, tff_REGISTER[0]);
    PRF("Output fval:%s\n", temp.f_val);
    PRF("Output lval:%s\n", temp.l_val);
}
