#include "KartMove.hpp"

extern "C" {
extern s16 lbl_1_data_30F2;
extern s16 lbl_1_data_30F6;
extern s16 lbl_1_data_2EE8;
extern f32 lbl_1_data_2F4C;
extern f32 lbl_1_rodata_22E0;
extern f32 lbl_1_data_2E24;
extern f32 lbl_1_data_2E28;
extern f32 lbl_1_data_2F08;

float PlayerSub10Bike_getLeanRot(void* self) {
  return *(f32*)((u8*)self + 0x294);
}

void PlayerSub10_setRegularHardSpeedLimit(void* self) {
  *(f32*)((u8*)self + 0x2C) = lbl_1_data_2E24;
}

void PlayerSub10_setBulletHardSpeedLimit(void* self) {
  *(f32*)((u8*)self + 0x2C) = lbl_1_data_2E28;
}

void PlayerSub10_activateTc(void* self) {
  u8* p = *(u8**)((u8*)self + 0x0);
  u8* q = *(u8**)(p + 0x4);
  *(u32*)(q + 0xC) |= 0x20000000u;
}

void PlayerSub10_deactivateTc(void* self) {
  u8* p = *(u8**)((u8*)self + 0x0);
  u8* q = *(u8**)(p + 0x4);
  *(u32*)(q + 0xC) &= ~0x20000000u;
}

float PlayerSub10Bike_getWheelieSoftSpeedLimitBonus(void* self) {
  u8* p = *(u8**)((u8*)self + 0x0);
  u8* q = *(u8**)(p + 0x4);
  if (*(u32*)(q + 0x4) & 0x20000000u) {
    return lbl_1_data_2F08;
  }
  return lbl_1_rodata_22E0;
}

void* PlayerSub10_destroy(void* self, int flags) {
  if (self != nullptr && flags > 0) {
    ::operator delete(self);
  }
  return self;
}

void* PlayerSub10Bike_destroy(void* self, int flags) {
  if (self != nullptr && flags > 0) {
    ::operator delete(self);
  }
  return self;
}

void* PlayerBoost_destroy(void* self, int flags) {
  if (self != nullptr && flags > 0) {
    ::operator delete(self);
  }
  return self;
}

void PlayerSub10_updateWheelie() {}
void PlayerSub10_cancelWheelie() {}
bool PlayerSub10_checkWheelie() { return false; }

void fn_1_6AEE8() {}
void fn_1_70080() {}
void fn_1_737D0() {}
void fn_1_787A0() {}
void fn_1_79048() {}
void fn_1_79304() {}

float PlayerSub10_getLeanRot() { return lbl_1_rodata_22E0; }

float PlayerSub10_getWheelieSoftSpeedLimitBonus() {
  return lbl_1_rodata_22E0;
}
}

namespace Kart {

void KartMove::empty(s32 arg) {}

s16 KartMove::getMaxMtCharge() { return lbl_1_data_30F2; }

s16 KartMove::getMaxSmtCharge() { return lbl_1_data_30F6; }

void KartMove::setScale(const EGG::Vector3f scale) { mScale = scale; }

} // namespace Kart

extern "C" s16 getStarDuration() { return lbl_1_data_2EE8; }
