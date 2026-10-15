#include "axAux.h"

#include <string.h>

#include <rvl/os/osCache.h>
#include <rvl/os/osInterrupt.h>

u32 _unk_803864dc;
u32 _unk_803864d8;
u32 _unk_803864d4;
u32 _unk_803864d0;
u32 _unk_803864cc;
u32 _unk_803864c8;
u32 _unk_803864c4;
u32 _unk_803864c0;
u32 _unk_803864bc;
u32 _unk_803864b8;
u32 _unk_803864b4;
u32 _unk_803864b0;
u32 _unk_803864ac;
u32 _unk_803864a8;
u32 _unk_803864a4;
u32 _unk_803864a0;
u32 _unk_8038649c;
u32 _unk_80386498;

extern u32 _unk_803864f0;

extern u8 lbl_802F41C0[];

void __AXGetAuxAInput(void** out) {
  if (_unk_803864d0 != 0) {
    *out = &lbl_802F41C0[_unk_803864a0 * 0x600];
  } else {
    *out = NULL;
  }
}

void __AXGetAuxAOutput(void** out) {
  *out = &lbl_802F41C0[_unk_8038649c * 0x600];
}

void __AXGetAuxAInputDpl2(void** out) {
  *out = (u8*)lbl_802F41C0 + _unk_803864a0 * 0x600 + 0x480;
}

void __AXGetAuxAOutputDpl2R(void** out) {
  *out = (u8*)lbl_802F41C0 + _unk_8038649c * 0x600 + 0x180;
}

void __AXGetAuxAOutputDpl2Ls(void** out) {
  *out = (u8*)lbl_802F41C0 + _unk_8038649c * 0x600 + 0x300;
}

void __AXGetAuxAOutputDpl2Rs(void** out) {
  *out = (u8*)lbl_802F41C0 + _unk_8038649c * 0x600 + 0x480;
}

extern u8 lbl_802F53C0[];
extern u8 lbl_802F65C0[];

void __AXGetAuxBInput(void** out) {
  if (_unk_803864cc != 0) {
    *out = &lbl_802F53C0[_unk_803864a0 * 0x600];
  } else {
    *out = NULL;
  }
}

void __AXGetAuxBOutput(void** out) {
  *out = &lbl_802F53C0[_unk_8038649c * 0x600];
}

void __AXGetAuxBInputDpl2(void** out) {
  *out = (u8*)lbl_802F53C0 + _unk_803864a0 * 0x600 + 0x480;
}

void __AXGetAuxBOutputDpl2R(void** out) {
  *out = (u8*)lbl_802F53C0 + _unk_8038649c * 0x600 + 0x180;
}

void __AXGetAuxBOutputDpl2Ls(void** out) {
  *out = (u8*)lbl_802F53C0 + _unk_8038649c * 0x600 + 0x300;
}

void __AXGetAuxBOutputDpl2Rs(void** out) {
  *out = (u8*)lbl_802F53C0 + _unk_8038649c * 0x600 + 0x480;
}

void __AXGetAuxCInput(void** out) {
  if (_unk_803864c8 != 0) {
    *out = &lbl_802F65C0[_unk_803864a0 * 0x480];
  } else {
    *out = NULL;
  }
}

void __AXGetAuxCOutput(void** out) {
  *out = &lbl_802F65C0[_unk_8038649c * 0x480];
}

void AXGetAuxACallback(u32* callback, u32* arg) {
  *callback = _unk_803864d0;
  *arg = _unk_803864c4;
}

void AXGetAuxBCallback(u32* callback, u32* arg) {
  *callback = _unk_803864cc;
  *arg = _unk_803864c0;
}

void AXGetAuxCCallback(u32* callback, u32* arg) {
  *callback = _unk_803864c8;
  *arg = _unk_803864bc;
}

void AXRegisterAuxACallback(void* callback, void* arg) {
  BOOL enabled = OSDisableInterrupts();
  _unk_803864d0 = (u32)callback;
  _unk_803864c4 = (u32)arg;
  if (callback == 0) {
    memset(&_unk_803864dc, 1, 3);
  }
  OSRestoreInterrupts(enabled);
}

void AXRegisterAuxBCallback(void* callback, void* arg) {
  BOOL enabled = OSDisableInterrupts();
  _unk_803864cc = (u32)callback;
  _unk_803864c0 = (u32)arg;
  if (callback == 0) {
    memset(&_unk_803864d8, 1, 3);
  }
  OSRestoreInterrupts(enabled);
}

void AXRegisterAuxCCallback(void* callback, void* arg) {
  BOOL enabled = OSDisableInterrupts();
  _unk_803864c8 = (u32)callback;
  _unk_803864bc = (u32)arg;
  if (callback == 0) {
    memset(&_unk_803864d4, 1, 3);
  }
  OSRestoreInterrupts(enabled);
}

