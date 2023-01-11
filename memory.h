#include <stdio.h>
#include <string.h>
#include "value_unit.h"

#define tffLoc BTS


TDF struct {
    tffLoc name_AREA;
    VU* AREA;
    size_t size_AREA;
    tffLoc* tab_AREA;
} tffAREA;


static struct {
    tffAREA* MEM;
    size_t size_MEM;
} tff_MEMORY;


// Functions Declarations
int tff_INIT(void);
int tff_malloc(VU);
int tff_assign(VU, VU);


int tff_INIT(void){
    if ((tff_MEMORY.MEM = (tffAREA*)MLC(SZF(tffAREA)))){
        tff_MEMORY.size_MEM = 1;
        tff_MEMORY.MEM[0].name_AREA = "N";
        tff_MEMORY.MEM[0].size_AREA = 1;
        if (
            (tff_MEMORY.MEM[0].AREA = (VU*)MLC(SZF(VU))) &&
            (tff_MEMORY.MEM[0].tab_AREA = (tffLoc*)MLC(SZF(tffLoc)))
        ){
            // (n, n) = [n, n]
            tff_MEMORY.MEM[0].tab_AREA[0] = "N";
            tff_MEMORY.MEM[0].AREA[0].f_val = "N";
            tff_MEMORY.MEM[0].AREA[0].l_val = "N";
            return 1;
        }
        else return 0;
    }
    else return 0;
}


int tff_malloc(VU loc){
    for (RGS i = 0; i < tff_MEMORY.size_MEM; i++){
        // Find tffAREA
        if (!strcmp(tff_MEMORY.MEM[i].name_AREA, loc.f_val)){
            for (RGS j = 0; j < tff_MEMORY.MEM[i].size_AREA; j++){
                // Find tffLoc, clear VU
                if (!strcmp(tff_MEMORY.MEM[i].tab_AREA[j], loc.l_val)){
                    tff_MEMORY.MEM[i].AREA[j].f_val = "N";
                    tff_MEMORY.MEM[i].AREA[j].l_val = "N";
                    return 1;
                }
            }
            // No tffLoc, create new tffLoc and VU
            if (tff_MEMORY.MEM[i].AREA = (VU*)RLC(tff_MEMORY.MEM[i].AREA, SZF(VU) * (tff_MEMORY.MEM[i].size_AREA+1))){
                tff_MEMORY.MEM[i].size_AREA++;
                tff_MEMORY.MEM[i].AREA[tff_MEMORY.MEM[i].size_AREA-1].f_val = "N";
                tff_MEMORY.MEM[i].AREA[tff_MEMORY.MEM[i].size_AREA-1].l_val = "N";
                if (tff_MEMORY.MEM[i].tab_AREA = (tffLoc*)RLC(tff_MEMORY.MEM[i].tab_AREA, SZF(tffLoc) * tff_MEMORY.MEM[i].size_AREA)){
                    tff_MEMORY.MEM[i].tab_AREA[tff_MEMORY.MEM[i].size_AREA-1] = loc.l_val;
                    return 1;
                }
                else return 0;
            }
            else return 0;
        }
    }
    // No tffAREA, create new tffAREA, tffLoc, and VU
    if (tff_MEMORY.MEM = (tffAREA*)RLC(tff_MEMORY.MEM, SZF(tffAREA) * (tff_MEMORY.size_MEM+1))){
        tff_MEMORY.size_MEM++;
        tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].name_AREA = loc.f_val;
        if (
            (tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].AREA = (VU*)MLC(SZF(VU))) &&
            (tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].tab_AREA = (tffLoc*)MLC(SZF(tffLoc)))
        ){
            tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].size_AREA = 1;
            tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].AREA[0].f_val = "N";
            tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].AREA[0].l_val = "N";
            tff_MEMORY.MEM[tff_MEMORY.size_MEM-1].tab_AREA[0] = loc.l_val;
            return 1;
        }
        else return 0;
    }
    else return 0;
}


int tff_assign(VU val, VU loc){
    if (tff_malloc(loc)){
        for (RGS i = 0; i < tff_MEMORY.size_MEM; i++){
            if (!strcmp(tff_MEMORY.MEM[i].name_AREA, loc.f_val)){
                for (RGS j = 0; j < tff_MEMORY.MEM[i].size_AREA; i++){
                    if (!strcmp(tff_MEMORY.MEM[i].tab_AREA[j], loc.l_val)){
                        tff_MEMORY.MEM[i].AREA[j].f_val = val.f_val;
                        tff_MEMORY.MEM[i].AREA[j].l_val = val.l_val;
                        return 1;
                    }
                }
                return 0;
            }
        }
        return 0;
    }
    else return 0;
}