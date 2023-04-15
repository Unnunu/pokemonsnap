#include "common.h"

s32 osPiGetCmdQueue(void) {
    if (D_80042D10 == 0) {
        return 0;
    } 
    return D_80042D18;
}