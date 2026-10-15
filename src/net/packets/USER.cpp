#include "USER.hpp"

namespace Net {}

extern "C" {

void fn_1_152748() {}

int fn_1_1528A4(void* self) { return *(u32*)((u8*)self + 0x9DC) == 0; }

int fn_1_1528B4(void* self, int i) {
  return (*(u32*)((u8*)self + 0x9E0) & (1u << i)) != 0;
}

}
