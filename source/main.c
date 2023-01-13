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

enum SCREEN_TYPE { TV = 1, DRC = 2, BOTH = 3 };

extern int CMPTLaunchTitle(void *CMPTConfigure, int ConfigSize, int titlehigh,
                           int titlelow);

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
