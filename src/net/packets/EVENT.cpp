#include "EVENT.hpp"

namespace Net {}

extern "C" {

void fn_1_14B378() {}

void fn_1_14BCFC(void* a, u32 b, u8* p) {
  *(u8*)(p + 0x1A) = b & 0x1F;
  *(u8*)(p + 0x1B) = (b >> 5) & 7;
}

}
