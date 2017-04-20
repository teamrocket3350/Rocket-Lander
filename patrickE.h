#ifndef patrickE
#define patrickE

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif

void UpgradeMenu(int xres,int yres,Rect r);
void BoosterMenu(int xres,int yres,Rect r);
void LaserMenu(int xres,int yres,Rect r);
#ifdef USE_OPENAL_SOUND
void playSound(ALuint source);
#endif

#endif
