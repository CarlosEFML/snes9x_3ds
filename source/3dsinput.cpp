
#include <3ds.h>
#include "3dsimpl.h"
#include "3dsgpu.h"
#include "3dssettings.h"

#include "inputRedirect.h"

static u32 currKeysHeld = 0;
static u32 lastKeysHeld = 0;


int sock2p = -1;
ControlIRED control2P = {.buttons = 0};

void initIRED() {
  static int initedIRED = -1;

  if (initedIRED == -1) initedIRED = initSysIRED();
  if ((initedIRED == 1) && (sock2p == -1)) sock2p = createUDPIRED();
}

void input3dsScanInputForEmulation2P() {
  if (sock2p != -1) recvUDPPackIRED(sock2p, &control2P);
}

u16 input3dsGetCurrentKeysHeld2P()
{
    return control2P.buttons;
}



//int adjustableValue = 0x70;

//---------------------------------------------------------
// Reads and processes Joy Pad buttons.
//
// This should be called only once every frame only in the
// emulator loop. For all other purposes, you should
// use the standard hidScanInput.
//---------------------------------------------------------
u32 input3dsScanInputForEmulation()
{
    hidScanInput();
    currKeysHeld = hidKeysHeld();

    u32 keysDown = (~lastKeysHeld) & currKeysHeld;

#ifndef RELEASE
    // -----------------------------------------------
    // For debug only
    // -----------------------------------------------
    if (GPU3DS.enableDebug)
    {
        keysDown = keysDown & (~lastKeysHeld);
        if (keysDown || (currKeysHeld & KEY_L))
        {
            //printf ("  kd:%x lkh:%x nkh:%x\n", keysDown, lastKeysHeld, currKeysHeld);
            //Settings.Paused = false;
        }
        else
        {
            //printf ("  kd:%x lkh:%x nkh:%x\n", keysDown, lastKeysHeld, currKeysHeld);
            //Settings.Paused = true;
        }
    }

    if (keysDown & (KEY_SELECT))
    {
        GPU3DS.enableDebug = !GPU3DS.enableDebug;
        printf ("Debug mode = %d\n", GPU3DS.enableDebug);
    }

    /*if (keysDown & (KEY_L))
    {
        adjustableValue -= 1;
        printf ("Adjust: %d\n", adjustableValue);
    }
    if (keysDown & (KEY_R))
    {
        adjustableValue += 1;
        printf ("Adjust: %d\n", adjustableValue);
    }*/
    // -----------------------------------------------
#endif

    if (keysDown & KEY_TOUCH ||
        (!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_OPEN_MENU].IsHeld(keysDown)) ||
        (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_OPEN_MENU].IsHeld(keysDown))
        )
    {
        impl3dsTouchScreenPressed();

        if (GPU3DS.emulatorState == EMUSTATE_EMULATE)
            GPU3DS.emulatorState = EMUSTATE_PAUSEMENU;
    }

    if (GPU3DS.emulatorState == EMUSTATE_EMULATE) {
        if ((!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_SWAP_CONTROLLERS].IsHeld(keysDown)) ||
            (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_SWAP_CONTROLLERS].IsHeld(keysDown)))
            impl3dsSwapJoypads();

        if ((!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_QUICK_SAVE].IsHeld(keysDown)) ||
            (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_QUICK_SAVE].IsHeld(keysDown)))
            impl3dsQuickSaveLoad(true);

        if ((!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_QUICK_LOAD].IsHeld(keysDown)) ||
            (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_QUICK_LOAD].IsHeld(keysDown)))
            impl3dsQuickSaveLoad(false);

        if ((!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_SAVE_SLOT_NEXT].IsHeld(keysDown)) ||
            (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_SAVE_SLOT_NEXT].IsHeld(keysDown)))
            impl3dsSelectSaveSlot(1);

        if ((!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_SAVE_SLOT_PREV].IsHeld(keysDown)) ||
            (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_SAVE_SLOT_PREV].IsHeld(keysDown)))
            impl3dsSelectSaveSlot(-1);

        if ((!settings3DS.UseGlobalEmuControlKeys && settings3DS.ButtonHotkeys[HOTKEY_SCREENSHOT].IsHeld(keysDown)) ||
            (settings3DS.UseGlobalEmuControlKeys && settings3DS.GlobalButtonHotkeys[HOTKEY_SCREENSHOT].IsHeld(keysDown))) {
            const char *path = NULL;
            impl3dsTakeScreenshot(path, false);
        }
    }


    lastKeysHeld = currKeysHeld;
    return keysDown;

}


//---------------------------------------------------------
// Get the bitmap of keys currently held on by the user
//---------------------------------------------------------
u32 input3dsGetCurrentKeysHeld()
{
    return currKeysHeld;
}
