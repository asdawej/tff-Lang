#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "__defines.h"
#include "value_unit.h"

#define tffLoc char*


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


int tff_INIT(void){}


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
            if (RLC(tff_MEMORY.MEM[i].AREA, SZF(VU) * (tff_MEMORY.MEM[i].size_AREA+1))){
                tff_MEMORY.MEM[i].size_AREA++;
                tff_MEMORY.MEM[i].AREA[tff_MEMORY.MEM[i].size_AREA-1].f_val = "N";
                tff_MEMORY.MEM[i].AREA[tff_MEMORY.MEM[i].size_AREA-1].l_val = "N";
                if (RLC(tff_MEMORY.MEM[i].tab_AREA, SZF(tffLoc) * tff_MEMORY.MEM[i].size_AREA)){
                    tff_MEMORY.MEM[i].tab_AREA[tff_MEMORY.MEM[i].size_AREA-1] = loc.l_val;
                    return 1;
                }
                else return 0;
            }
            else return 0;
        }
    }
    // No tffAREA, create new tffAREA, tffLoc, and VU
    if (RLC(tff_MEMORY.MEM, SZF(tffAREA) * (tff_MEMORY.size_MEM+1))){
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
