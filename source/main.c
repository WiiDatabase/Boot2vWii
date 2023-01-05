#include <malloc.h>
#include <stdlib.h>
#include <padscore/kpad.h>
#include <coreinit/thread.h>
#include <whb/proc.h>

#ifndef TIDHIGH
#define TIDHIGH 0x00010001
#endif

#ifndef TIDLOW
#define TIDLOW 0x44434f41 // DCOA (RC24 ConnectMii)
#endif

#ifndef DISPLAY
#define DISPLAY BOTH
#endif

#ifndef FORCERES
#define FORCERES NONE
#endif

enum SCREEN_TYPE {
    TV = 1,
    DRC = 2,
    BOTH = 3
};

enum FORCE_RES {
    NONE = 0,
    P720 = 4,
    P480 = 3
};

extern int (OSDynLoad_Acquire)(const char* rpl, uint32_t *handle);

extern int (OSDynLoad_FindExport)(uint32_t handle, int isdata, const char *symbol, void *address);

void SwitchVideoMode() {
		
    unsigned int avm_handle;
    OSDynLoad_Acquire("avm.rpl", & avm_handle);
    int( * AVMSetTVScanResolution)(int r3);
    int( * AVMGetCurrentPort)(int * port);

    OSDynLoad_FindExport(avm_handle, 0, "AVMSetTVScanResolution", & AVMSetTVScanResolution);
    OSDynLoad_FindExport(avm_handle, 0, "AVMGetCurrentPort", & AVMGetCurrentPort);

    // out port shouldn't be needed, dont use if scart/composite or component
    int outPort = 0;
    AVMGetCurrentPort( & outPort);
    if (outPort > 3) outPort = 0;
    int wantRes = FORCERES;

    if (outPort == 2 || outPort == 3) {
        return;
    } else {
        AVMSetTVScanResolution(wantRes);
    }

}

extern int CMPTLaunchTitle(void *CMPTConfigure, int ConfigSize, int titlehigh, int titlelow);

extern int CMPTAcctSetScreenType(int screenType);

extern int CMPTGetDataSize(int *dataSize);

extern int CMPTCheckScreenState();

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

    int dataSize;
    int result = CMPTGetDataSize(&dataSize);
    if (result < 0) {
        return 0;
    }

    KPADInit();
	
	if (FORCERES != NONE) {
	SwitchVideoMode();
	}

    void *databuf = memalign(0x40, dataSize);
    CMPTLaunchTitle(databuf, dataSize, TIDHIGH, TIDLOW);
    free(databuf);

    WHBProcInit();
    while(WHBProcIsRunning()) {     
       OSSleepTicks(OSMillisecondsToTicks(100));
    }
    WHBProcShutdown();

    return 0;
}
