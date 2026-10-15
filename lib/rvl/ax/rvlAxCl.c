#include "cl.h"

#include "axAux.h"

#include <rvl/os/osCache.h>

#include "spb.h"
#include "vpb.h"

u16 _unk_802f7340[2][64] __attribute__((aligned(32)));

u32 _unk_803864f8;
u32 _unk_803864f4;
u32 _unk_803864f0;
u32 _unk_803864ec;
u16 _unk_803864e8;
u16 _unk_803864e6;
u16 _unk_803864e4;
u16 _unk_803864e2;
u16 _unk_803864e0;

u32 __AXGetCommandListCycles() { return _unk_803864ec; }

void AXSetMode(u32 mode) { _unk_803864f0 = mode; }

u32 AXGetMode() { return _unk_803864f0; }

u16 AXGetAuxAReturnVolume() { return _unk_803864e4; }

u16 AXGetAuxBReturnVolume() { return _unk_803864e2; }

u16 AXGetAuxCReturnVolume() { return _unk_803864e0; }

void AXSetAuxAReturnVolume(u16 vol) { _unk_803864e4 = vol; }

void AXSetAuxBReturnVolume(u16 vol) { _unk_803864e2 = vol; }

void AXSetAuxCReturnVolume(u16 vol) { _unk_803864e0 = vol; }

void AXSetMasterVolume(u32 vol) {
  if (vol > 0x8000) {
    vol = 0x8000;
  }
  _unk_803864e6 = vol;
}
