#include "prof.h"

u32 __AXProfileInitialized = 0;
u32 __AXCurrentProfile = 0;
u32 __AXMaxProfiles = 0;
u32 __AXProfile = 0;

void* __AXGetCurrentProfile() {
  if (__AXProfileInitialized == 0) {
    return NULL;
  }

  u32 profile = __AXCurrentProfile;
  void* result = (void*)(__AXProfile + profile * 0x38);
  __AXCurrentProfile = (profile + 1) % __AXMaxProfiles;

  return result;
}
