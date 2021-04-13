#include <malloc.h>
#include <stdlib.h>
#include <padscore/kpad.h>

#ifndef TIDHIGH
#define TIDHIGH 0x00010001
#endif

#ifndef TIDLOW
#define TIDLOW 0x44434f41 // DCOA (RC24 ConnectMii)
#endif

extern int CMPTLaunchTitle(void *CMPTConfigure, int ConfigSize, int titlehigh, int titlelow);

extern int CMPTAcctSetScreenType(int screenType);

extern int CMPTGetDataSize(int *dataSize);

extern int CMPTCheckScreenState();

int main(int argc, char **argv) {
    CMPTAcctSetScreenType(3);
    if (CMPTCheckScreenState() < 0) {
        CMPTAcctSetScreenType(2);
        if (CMPTCheckScreenState() < 0) {
            CMPTAcctSetScreenType(1);
        }
    }

    int dataSize;
    int result = CMPTGetDataSize(&dataSize);
    if (result < 0) {
        return 0;
    }

    KPADInit();

    void *databuf = memalign(0x40, dataSize);
    CMPTLaunchTitle(databuf, dataSize, TIDHIGH, TIDLOW);
    free(databuf);

    return 0;
}
