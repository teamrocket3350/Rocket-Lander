#ifndef patrickE
#define patrickE

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif

void UpgradeMenu(Rect r);
void BoosterMenu(Rect r);
void LaserMenu(Rect r);
void startMenu(Rect r);
void renderMenu(void);
void menus(Rect r);
void menus2(Rect r);
#ifdef USE_OPENAL_SOUND
void playSound(ALuint source);
#endif

#endif
