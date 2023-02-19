#include "memory.h"
#include "__assist.h"


static const char tff_CACHEFILE[] = "__tff_cache__.tffb";
static const char tff_TYPE_FUNC[] = "N";


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
// In tff_PROCESS_STACK, there are {'2', '3', '4', '5', '6', '7', '8'}.
// In tff_BREAKPOINT_STACK, there are the positions to store.
// tff_REGISTER is for VU input.
static char tff_PROCESS_STACK[128] = {0};
static fpos_t tff_BREAKPOINT_STACK[4096] = {0};
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
// tff_flag_args used in meth_3/5/6, because they have 2 or more args.
// [VU] and [SS] are all percepted as args.
// To extract an arg, we need tff_flag_argindex and tff_flag_flval
//     to locate them in the tff_REGISTER or tff_BREAKPOINT_STACK.
static int tff_flag_args = 0;       //                      0: Not args;        1: Args.
static int tff_flag_argindex = -1;  // -1: not arg;         0: The first arg;   1: The second arg   2: The third arg.
static int tff_flag_flval = -1;     // -1: not arg;         0: The fval;        1: The lval.
static int tff_count_lrclose = 0;   // To count if 0-1 pair is closed to check whether a section of code is over.
static size_t tff_count_val = 0;    // To count the number of f_val/l_val.


// Functions Declarations
static void tff_meth_2(void);
static void tff_meth_3(void);
static void tff_meth_4(FILE*);
static void tff_meth_5(FILE*);
static void tff_meth_6(FILE*);
static void tff_meth_7(void);
static void tff_meth_8(void);
static void tff_run(FILE*);
static void tff_local(FILE*);
static void tff_record(FILE*);


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
    rfp = FOPN(tff_CACHEFILE, "r");
    tff_run(rfp);
    FCLS(rfp);
    return 0;
}


static void tff_meth_2(void){
    tff_getval(&tff_REGISTER[tff_flag_argindex], tff_REGISTER[tff_flag_argindex]);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_meth_3(void){
    tff_assign(tff_REGISTER[1], tff_REGISTER[0]);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_meth_4(FILE* rfp){
    fgetpos(rfp, &tff_BREAKPOINT_STACK[++tff_BREAKPOINT_STACK[0]]);
    fpos_t temp = BTS_AsDeci(tff_REGISTER[0].l_val); fsetpos(rfp, &temp);
    tff_local(rfp);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_meth_5(FILE* rfp){
    VU temp = {
        .f_val = tff_TYPE_FUNC,
        .l_val = BTS_FromDeci(tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]])
    };
    tff_assign(temp, tff_REGISTER[0]);
    tff_BREAKPOINT_STACK[0]--;
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_meth_6(FILE* rfp){
    if (tff_REGISTER[0].l_val[0] == 'T'){
        tff_BREAKPOINT_STACK[0]--;
        tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]+1] = tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]];
        fgetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]]);
        fsetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]+1]);
        tff_local(rfp);
    }
    else if (tff_REGISTER[0].l_val[0] == 'F'){
        tff_BREAKPOINT_STACK[0]--;
        fgetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]]);
        fsetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]+1]);
        tff_local(rfp);
    }
    else if (tff_REGISTER[0].l_val[0] == 'N'){
        tff_BREAKPOINT_STACK[0] -= 2;
    }
    else exit(-2);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_meth_7(void){
    VU temp;
    char c;
    _INPUT_f:
        temp.f_val = (BTS)MLC(1); temp.f_val[0] = '\0';
        size_t _fsave = 0;
        PRF("Input fval: ");
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
        PRF("Input lval: ");
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
    temp.f_val = BTS_Simpl(temp.f_val);
    temp.l_val = BTS_Simpl(temp.l_val);
    tff_assign(temp, tff_REGISTER[0]);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_meth_8(void){
    VU temp;
    tff_getval(&temp, tff_REGISTER[0]);
    PRF("Output fval: %s\n", temp.f_val);
    PRF("Output lval: %s\n", temp.l_val);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_run(FILE* rfp){
    char c;
    while ((c = fgetc(rfp)) != EOF){
        // Push process stack
        for (RGT i = 5; i < tff_FUNCTAB_size; i++){
            if (c == tff_FUNCTAB[i]){
                tff_PROCESS_STACK[++tff_PROCESS_STACK[0]] = c;
                goto _RUN_LOOP_END;
            }
        }
        // In 2 field
        if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '2'){
            if (c == '0'){
                // Start to extract f_val/l_val
                if (forechar(rfp) != '2'){
                    tff_flag_flval++;
                    if (tff_flag_flval == 0){
                        if (tff_REGISTER[tff_flag_argindex].f_val = (BTS)MLC(1)){
                            tff_REGISTER[tff_flag_argindex].f_val[0] = '\0';
                            tff_count_val = 1;
                        }
                        else exit(-1);
                    }
                    else if (tff_flag_flval == 1){
                        if (tff_REGISTER[tff_flag_argindex].l_val = (BTS)MLC(1)){
                            tff_REGISTER[tff_flag_argindex].l_val[0] = '\0';
                            tff_count_val = 1;
                        }
                        else exit(-1);
                    }
                }
            }
            else if (c == '1'){
                // Run meth 2
                if (forechar(rfp) == '1'){
                    tff_meth_2();
                }
            }
            // Extract f_val/l_val
            else if (c == 'T' || c == 'N' || c == 'F'){
                if (tff_flag_flval == 0){
                    if (tff_REGISTER[tff_flag_argindex].f_val = (BTS)RLC(tff_REGISTER[tff_flag_argindex].f_val, ++tff_count_val)){
                        tff_REGISTER[tff_flag_argindex].f_val[tff_count_val-2] = c;
                    }
                    else exit(-1);
                }
                else if (tff_flag_flval == 1){
                    if (tff_REGISTER[tff_flag_argindex].l_val = (BTS)RLC(tff_REGISTER[tff_flag_argindex].l_val, ++tff_count_val)){
                        tff_REGISTER[tff_flag_argindex].l_val[tff_count_val-2] = c;
                    }
                    else exit(-1);
                }
            }
        }
        // In 3 field
        else if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '3'){

        }
        // In 4 field
        else if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '4'){
            
        }
        // In 5 field
        else if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '5'){
            
        }
        // In 6 field
        else if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '6'){
            
        }
        // In 7 field
        else if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '7'){
            
        }
        // In 8 field
        else if (tff_PROCESS_STACK[tff_PROCESS_STACK[0]] == '8'){
            
        }
        _RUN_LOOP_END:
    }
}


static void tff_local(FILE* rfp){}


static void tff_record(FILE* rfp){}
