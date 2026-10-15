#include "SELECT.hpp"

namespace Net {

const u8* SELECTHandler::getPlayerIdToAidMapping() const {
  return m_sendPacket.m_playerIdToAidMapping;
}

} // namespace Net

extern "C" {

void fn_1_14FF44() {}

int fn_1_150978(void* self) { return *(u32*)((u8*)self + 0x2C) != 0; }

void fn_1_15039C(void* self, u8 a, u8 b, u8 c, int i, u8 d) {
  u8* p = (u8*)self + i * 8;
  *(u8*)(p + 0x1C) = a;
  *(u8*)(p + 0x1D) = b;
  *(u8*)(p + 0x1E) = c;
  *(u8*)(p + 0x1F) = d;
}

}
