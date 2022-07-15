
#ifndef _3DSINPUT_H
#define _3DSINPUT_H

void initIRED();
void input3dsScanInputForEmulation2P();
u16 input3dsGetCurrentKeysHeld2P();

//---------------------------------------------------------
// Reads and processes Joy Pad buttons.
//
// This should be called only once every frame only in the
// emulator loop. For all other purposes, you should
// use the standard hidScanInput.
//---------------------------------------------------------
u32 input3dsScanInputForEmulation();

//---------------------------------------------------------
// Get the bitmap of keys currently held on by the user
//---------------------------------------------------------
u32 input3dsGetCurrentKeysHeld();

#endif
