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
    '7',// Input
    '8',// Output
    '9' // Add
}; const int tff_FUNCTAB_size = 13;


// tff_PROCESS_STACK[0] and tff_BREAKPOINT_STACK[0] store lengths.
// In tff_PROCESS_STACK, there are {0, '2', '3', '4', '5', '6', '7', '8', '9'}.
// tff_PROCESS_STACK {0} is used for tff_local.
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
static int tff_flag_argval = 0;     //                      0: Not vals;        1: Vals.
static int tff_flag_flval = -1;     // -1: not arg;         0: The fval;        1: The lval.
static size_t tff_count_val = 0;    // To count the number of f_val/l_val.


// Functions Declarations
static void tff_meth_2(void);
static void tff_meth_3(void);
static void tff_meth_4(FILE*);
static void tff_meth_5(FILE*);
static void tff_meth_6(FILE*);
static void tff_meth_7(void);
static void tff_meth_8(void);
static void tff_meth_9(void);
static void tff_run(FILE*);
static void tff_local(FILE*);
static void tff_record(FILE*);


// Internal Execution Bodies Declarations
static int _tff_exec_push_stack(char);
static void _tff_exec_field_2(FILE*, char);
static void _tff_exec_field_39(FILE*, char);
static void _tff_exec_field_478(FILE*, char);
static void _tff_exec_field_5(FILE*, char);
static void _tff_exec_field_6(FILE*, char);
static void _tff_exec_start_flval(void);
static void _tff_exec_extract_flval(char);


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
    system("pause");
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
    VU temp_vu; tff_getval(&temp_vu, tff_REGISTER[0]);
    fpos_t temp_pos = BTS_AsDeci(temp_vu.l_val); fsetpos(rfp, &temp_pos);
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
        temp.f_val = (BTS)MLC(SZ_char); temp.f_val[0] = '\0';
        size_t _fsave = 0;
        PRF("Input fval: ");
        while ((c = GCR()) != '\n'){
            if ((c != 'T') && (c != 'N') && (c != 'F')){
                free(temp.f_val);
                fflush(stdin);
                goto _INPUT_f;
            }
            temp.f_val[_fsave] = c;
            if (temp.f_val = (BTS)RLC(temp.f_val, SZ_char * ((++_fsave)+1))){
                temp.f_val[_fsave] = '\0';
            }
            else exit(-1);
        }
    _INPUT_l:
        temp.l_val = (BTS)MLC(SZ_char); temp.l_val[0] = '\0';
        size_t _lsave = 0;
        PRF("Input lval: ");
        while ((c = GCR()) != '\n'){
            if ((c != 'T') && (c != 'N') && (c != 'F')){
                free(temp.l_val);
                fflush(stdin);
                goto _INPUT_l;
            }
            temp.l_val[_lsave] = c;
            if (temp.l_val = (BTS)RLC(temp.l_val, SZ_char * ((++_lsave)+1))){
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


static void tff_meth_9(void){
    VU temp; Deci temp_f, temp_l;
    tff_getval(&temp, tff_REGISTER[0]);
    temp_f = BTS_AsDeci(temp.f_val)+BTS_AsDeci(tff_REGISTER[1].f_val);
    temp_l = BTS_AsDeci(temp.l_val)+BTS_AsDeci(tff_REGISTER[1].l_val);
    temp.f_val = BTS_FromDeci(temp_f);
    temp.l_val = BTS_FromDeci(temp_l);
    tff_assign(temp, tff_REGISTER[0]);
    tff_PROCESS_STACK[0]--; // Pop stack
}


static void tff_run(FILE* rfp){
    char c;
    while ((c = fgetc(rfp)) != EOF){
        // Push process stack
        if (_tff_exec_push_stack(c)) continue;

        switch (tff_PROCESS_STACK[tff_PROCESS_STACK[0]]){
            case '2': _tff_exec_field_2(rfp, c); break;
            case '3':
            case '9': _tff_exec_field_39(rfp, c); break;
            case '4':
            case '7':
            case '8': _tff_exec_field_478(rfp, c); break;
            case '5': _tff_exec_field_5(rfp, c); break;
            case '6': _tff_exec_field_6(rfp, c);
            default: break;
        }
    }
}


static void tff_local(FILE* rfp){
    char c;
    LL tff_count_lrclose = 0;
    tff_PROCESS_STACK[++tff_PROCESS_STACK[0]] = 0;  // Avoid the last '1' meets with the stack '4'
    while ((c = fgetc(rfp)) != EOF){
        // Push process stack
        if (_tff_exec_push_stack(c)) continue;

        switch (tff_PROCESS_STACK[tff_PROCESS_STACK[0]]){
            case '2': _tff_exec_field_2(rfp, c); break;
            case '3':
            case '9': _tff_exec_field_39(rfp, c); break;
            case '4':
            case '7':
            case '8': _tff_exec_field_478(rfp, c); break;
            case '5': _tff_exec_field_5(rfp, c); break;
            case '6': _tff_exec_field_6(rfp, c);
            default: break;
        }

        if (c == '0') tff_count_lrclose++;
        else if (c == '1') tff_count_lrclose--;
        if (tff_count_lrclose == 0) break;
    }
    tff_PROCESS_STACK[0]--;
    fsetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]]);
    tff_BREAKPOINT_STACK[0]--;
}


static void tff_record(FILE* rfp){
    char c;
    LL tff_count_lrclose = 0;
    while ((c = fgetc(rfp)) != EOF){
        if (c == '0') tff_count_lrclose++;
        else if (c == '1') tff_count_lrclose--;
        if (tff_count_lrclose == 0) break;
    }
    fpos_t temp;
    fgetpos(rfp, &temp);
    temp--;
    fsetpos(rfp, &temp);
}


static int _tff_exec_push_stack(char c){
    for (RGT i = 5; i < tff_FUNCTAB_size; i++){
        if (c == tff_FUNCTAB[i]){
            tff_PROCESS_STACK[++tff_PROCESS_STACK[0]] = c;
            return 1;
        }
    }
    return 0;
}


static void _tff_exec_field_2(FILE* rfp, char c){
    if (c == '0'){
        // Start to extract f_val/l_val
        if (forechar(rfp) != '2'){
            _tff_exec_start_flval();
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
        _tff_exec_extract_flval(c);
    }
}


static void _tff_exec_field_39(FILE* rfp, char c){
    if (c == '0'){
        // Start args
        if (tff_flag_args == 0){
            tff_flag_args = 1;
        }
        else{
            // Start arg input
            if (tff_flag_argval == 0){
                tff_flag_argval = 1;
                tff_flag_argindex++;
            }
            // Start to extract f_val/l_val
            else _tff_exec_start_flval();
        }
    }
    else if (c == '1'){
        // End args
        if (tff_flag_argindex == 1 && tff_flag_argval == 0){
            tff_flag_args = 0;
            tff_flag_argindex = -1;
            switch (tff_PROCESS_STACK[tff_PROCESS_STACK[0]]){
                case '3': tff_meth_3(); break;
                case '9': tff_meth_9();
                default: break;
            }
        }
        else{
            // End arg input
            if (forechar(rfp) == '1'){
                tff_flag_flval = -1;
                tff_flag_argval = 0;
            }
        }
    }
    // Extract f_val/l_val
    else if (c == 'T' || c == 'N' || c == 'F'){
        _tff_exec_extract_flval(c);
    }
}


static void _tff_exec_field_478(FILE* rfp, char c){
    if (c == '0'){
        // Start args & arg input
        if (tff_flag_args == 0){
            tff_flag_args = 1;
            tff_flag_argval = 1;
            tff_flag_argindex++;
        }
        // Start to extract f_val/l_val
        else _tff_exec_start_flval();
    }
    else if (c == '1'){
        // End args & arg input
        if (forechar(rfp) == '1'){
            tff_flag_args = 0;
            tff_flag_argindex = -1;
            tff_flag_argval = 0;
            tff_flag_flval = -1;
            switch (tff_PROCESS_STACK[tff_PROCESS_STACK[0]]){
                case '4': tff_meth_4(rfp); break;
                case '7': tff_meth_7(); break;
                case '8': tff_meth_8();
                default: break;
            }
        }
    }
    // Extract f_val/l_val
    else if (c == 'T' || c == 'N' || c == 'F'){
        _tff_exec_extract_flval(c);
    }
}


static void _tff_exec_field_5(FILE* rfp, char c){
    if (c == '0'){
        // Start args
        if (tff_flag_args == 0){
            tff_flag_args = 1;
        }
        else{
            // Start arg input
            if (tff_flag_argval == 0){
                tff_flag_argval = 1;
                tff_flag_argindex++;
                // Input SS arg
                if (tff_flag_argindex == 1){
                    fpos_t temp; fgetpos(rfp, &temp);
                    tff_BREAKPOINT_STACK[++tff_BREAKPOINT_STACK[0]] = temp-1;   // To make sure the first char in tff_record is '0'
                    fsetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]]);
                    tff_record(rfp);
                }
            }
            // Start to extract f_val/l_val
            else _tff_exec_start_flval();
        }
    }
    else if (c == '1'){
        // End args
        if (tff_flag_argindex == 1 && tff_flag_argval == 0){
            tff_flag_args = 0;
            tff_flag_argindex = -1;
            tff_meth_5(rfp);
        }
        else{
            // End arg input
            if (forechar(rfp) == '1'){
                tff_flag_flval = -1;
                tff_flag_argval = 0;
            }
        }
    }
    // Extract f_val/l_val
    else if (c == 'T' || c == 'N' || c == 'F'){
        _tff_exec_extract_flval(c);
    }
}


static void _tff_exec_field_6(FILE* rfp, char c){
    if (c == '0'){
        // Start args
        if (tff_flag_args == 0){
            tff_flag_args = 1;
        }
        else{
            // Start arg input
            if (tff_flag_argval == 0){
                tff_flag_argval = 1;
                tff_flag_argindex++;
                // Input SS arg
                if (tff_flag_argindex == 1 || tff_flag_argindex == 2){
                    fpos_t temp; fgetpos(rfp, &temp);
                    tff_BREAKPOINT_STACK[++tff_BREAKPOINT_STACK[0]] = temp-1;   // To make sure the first char in tff_record is '0'
                    fsetpos(rfp, &tff_BREAKPOINT_STACK[tff_BREAKPOINT_STACK[0]]);
                    tff_record(rfp);
                }
            }
            // Start to extract f_val/l_val
            else _tff_exec_start_flval();
        }
    }
    else if (c == '1'){
        // End args
        if (tff_flag_argindex == 2 && tff_flag_argval == 0){
            tff_flag_args = 0;
            tff_flag_argindex = -1;
            tff_meth_6(rfp);
        }
        else{
            // End arg input
            if (forechar(rfp) == '1'){
                tff_flag_flval = -1;
                tff_flag_argval = 0;
            }
        }
    }
    // Extract f_val/l_val
    else if (c == 'T' || c == 'N' || c == 'F'){
        _tff_exec_extract_flval(c);
    }
}


static void _tff_exec_start_flval(void){
    tff_flag_flval++;
    if (tff_flag_flval == 0){
        if (tff_REGISTER[tff_flag_argindex].f_val = (BTS)MLC(SZ_char)){
            tff_REGISTER[tff_flag_argindex].f_val[0] = '\0';
            tff_count_val = 1;
        }
        else exit(-1);
    }
    else if (tff_flag_flval == 1){
        if (tff_REGISTER[tff_flag_argindex].l_val = (BTS)MLC(SZ_char)){
            tff_REGISTER[tff_flag_argindex].l_val[0] = '\0';
            tff_count_val = 1;
        }
        else exit(-1);
    }
}


static void _tff_exec_extract_flval(char c){
    if (tff_flag_flval == 0){
        if (tff_REGISTER[tff_flag_argindex].f_val = (BTS)RLC(tff_REGISTER[tff_flag_argindex].f_val, SZ_char * (++tff_count_val))){
            tff_REGISTER[tff_flag_argindex].f_val[tff_count_val-2] = c;
            tff_REGISTER[tff_flag_argindex].f_val[tff_count_val-1] = '\0';
        }
        else exit(-1);
    }
    else if (tff_flag_flval == 1){
        if (tff_REGISTER[tff_flag_argindex].l_val = (BTS)RLC(tff_REGISTER[tff_flag_argindex].l_val, SZ_char * (++tff_count_val))){
            tff_REGISTER[tff_flag_argindex].l_val[tff_count_val-2] = c;
            tff_REGISTER[tff_flag_argindex].l_val[tff_count_val-1] = '\0';
        }
        else exit(-1);
    }
}
