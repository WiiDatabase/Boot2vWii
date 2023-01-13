#include <coreinit/thread.h>
#include <malloc.h>
#include <padscore/kpad.h>
#include <stdlib.h>
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

enum SCREEN_TYPE { TV = 1, DRC = 2, BOTH = 3 };
enum FORCERES { NONE = 0, P720 = 4, P480 = 3 };

extern void AVMGetCurrentPort(int *port);
extern void AVMSetTVScanResolution(int res);

extern int CMPTLaunchTitle(void *CMPTConfigure, int ConfigSize, int titlehigh,
                           int titlelow);

extern int CMPTAcctSetScreenType(int screenType);

extern int CMPTGetDataSize(int *dataSize);

extern int CMPTCheckScreenState();

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

  int dataSize;
  int result = CMPTGetDataSize(&dataSize);
  if (result < 0) {
    return 0;
  }

  KPADInit();

  if (FORCERES != NONE) {
    set_video_mode();
  }

  void *databuf = memalign(0x40, dataSize);
  CMPTLaunchTitle(databuf, dataSize, TIDHIGH, TIDLOW);
  free(databuf);

  WHBProcInit();
  while (WHBProcIsRunning()) {
    OSSleepTicks(OSMillisecondsToTicks(100));
  }
  WHBProcShutdown();

  return 0;
}
