#include <coreinit/thread.h>
#include <malloc.h>
#include <nn/cmpt/cmpt.h>
#include <padscore/kpad.h>
#include <stdlib.h>
#include <whb/proc.h>

#ifndef TID
#define TID 0
#endif

#ifndef DISPLAY
#define DISPLAY BOTH
#endif

#ifndef FORCERES
#define FORCERES NONE
#endif

enum SCREEN_TYPE { TV   = 1,
                   DRC  = 2,
                   BOTH = 3 };
enum FORCERES { NONE = 0,
                P720 = 4,
                P480 = 3 };

extern void AVMGetCurrentPort(int *port);
extern void AVMSetTVScanResolution(int res);

void set_video_mode() {
    int outPort = 0;
    AVMGetCurrentPort(&outPort);

    if (outPort > 3) {
        outPort = 0;
    }
    int wantRes = FORCERES;

    if (outPort >= 2) {
        return;
    } else {
        AVMSetTVScanResolution(wantRes);
    }
}

int main(int argc, char **argv) {
    CMPTAcctSetScreenType(DISPLAY);

    int screenState = CMPTCheckScreenState();
    if (screenState < 0) {
        if (screenState == -9) { // HDMI error
            CMPTAcctSetScreenType(DRC);
        } else {
            return 0;
        }
    }

    uint32_t dataSize;
    int result = CMPTGetDataSize(&dataSize);
    if (result < 0) {
        return 0;
    }

    KPADInit();

    if (FORCERES != NONE) {
        set_video_mode();
    }

    void *databuf = memalign(0x40, dataSize);
    if (TID == 0) {
        CMPTLaunchMenu(databuf, dataSize);
    } else {
        CMPTLaunchTitle(databuf, dataSize, TID);
    }
    free(databuf);

    WHBProcInit();
    while (WHBProcIsRunning()) {
        OSSleepTicks(OSMillisecondsToTicks(100));
    }
    WHBProcShutdown();

    return 0;
}
