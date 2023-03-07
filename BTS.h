#include <stdlib.h>
#include "__defines.h"

#define BTS char*
#define Deci LL

const size_t SZ_char = SZF(char);


Deci BTS_unit_AsDeci(char unit){
    switch (unit){
        case 'T': return 1;
        case 'N': return 0;
        case 'F': return -1;
        default: break;
    }
}


Deci BTS_AsDeci(BTS BTS_val){
    Deci Deci_val = 0;
    for (RGS i = 0; BTS_val[i] != '\0'; i++){
        Deci_val *= 2;
        Deci_val += BTS_unit_AsDeci(BTS_val[i]);
    }
    return Deci_val;
}


BTS BTS_FromDeci(Deci Deci_val){
    if (Deci_val == 0){
        return "N";
    }
    else if (Deci_val > 0){
        BTS ret = (BTS)MLC(SZ_char);
        ret[0] = '\0';
        LL length = 1;
        Deci m;
        while ((m = Deci_val/2) >= 1){
            if (ret = (BTS)RLC(ret, SZ_char * (++length))){
                for (RG LL i = length-2; i >= 0; i--){
                    ret[i+1] = ret[i];
                }
                if (Deci_val-2*m){
                    ret[0] = 'T';
                }
                else ret[0] = 'N';
                Deci_val = m;
            }
            else return NULL;
        }
        if (ret = (BTS)RLC(ret, SZ_char * (++length))){
            for (RG LL i = length-2; i >= 0; i--){
                ret[i+1] = ret[i];
            }
            ret[0] = 'T';
        }
        else return NULL;
        return ret;
    }
    else{
        BTS ret = BTS_FromDeci(-Deci_val);
        for (RGS i = 0; ret[i] != '\0'; i++){
            if (ret[i] == 'T') ret[i] = 'F';
        }
        return ret;
    }
}


BTS BTS_Simpl(BTS val){
    return BTS_FromDeci(BTS_AsDeci(val));
}
