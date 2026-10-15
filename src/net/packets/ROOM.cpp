#include "ROOM.hpp"

namespace Net {}

extern "C" {

void* fn_1_14ABB0(void* self, int i) {
  return *(void**)((u8*)self + i * 4 + 0x38);
}

void fn_1_14ABC0(void* self, int i, u32 v) {
  *(u32*)((u8*)self + i * 4 + 0x8) = v;
}

void fn_1_14AF2C(void* self, u32* p) {
  *p = 0;
}

void fn_1_14AF38(void* self, int i) {
  *(u32*)((u8*)self + i * 4 + 0x8) = 0;
}

void fn_1_14AF4C(void* self, int i) {
  *(u32*)((u8*)self + i * 4 + 0x38) = 0;
}

void fn_1_14AF60(void* self, int i, u32 v) {
  *(u32*)((u8*)self + i * 4 + 0x8) = v;
}

void fn_1_14AF70(void* self, int i, u32 v) {
  *(u32*)((u8*)self + i * 4 + 0x38) = v;
}

void* fn_1_14AF80(void* self, int i) {
  return (u8*)self + i * 4 + 0x8;
}

void* fn_1_14AF90(void* self, int i) {
  return (u8*)self + i * 4 + 0x38;
}

}
