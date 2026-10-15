#include "axfx.h"

#include <rvl/os/osAlloc.h>

AXFXAlloc __AXFXAlloc = __AXFXAllocFunction;
AXFXFree __AXFXFree = __AXFXFreeFunction;

void* __AXFXAllocFunction(u32 size) {
  return OSAllocFromHeap(__OSCurrentHeap, size);
}

void __AXFXFreeFunction(void* block) {
  OSFreeToHeap(__OSCurrentHeap, block);
}

void AXFXSetHooks(AXFXAlloc alloc, AXFXFree free) {
  __AXFXAlloc = alloc;
  __AXFXFree = free;
}

void AXFXGetHooks(AXFXAlloc* alloc, AXFXFree* free) {
  *alloc = __AXFXAlloc;
  *free = __AXFXFree;
}
