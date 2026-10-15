#include "alloc.h"

#include <rvl/os/osInterrupt.h>

#include "vpb.h"

u32 _unk_802f40c0[32];
u32 _unk_802f4140[32];

u32 _unk_80386490;

u32 __AXGetStackHead(u32 i) { return _unk_802f40c0[i]; }

void __AXPushFreeStack(void* node) {
  *(u32*)node = _unk_802f40c0[0];
  _unk_802f40c0[0] = (u32)node;
  *(u32*)((u8*)node + 0xc) = 0;
}

void __AXPushCallbackStack(void* node) {
  *(u32*)((u8*)node + 0x8) = _unk_80386490;
  _unk_80386490 = (u32)node;
}
