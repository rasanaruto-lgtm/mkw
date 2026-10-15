#include "KartObjectProxy.hpp"
#include "egg/math/eggMatrix.hpp"
#include "host_system/SystemManager.hpp"
#include "kart/KartPart.hpp"
#include "kart/KartPhysics.hpp"
#include "kart/KartState.hpp"
#include "system/KPadController.hpp"
#include "system/RaceManager.hpp"
#include <stddef.h>

#include <kart/KartMove.hpp>
#include <kart/KartBody.hpp>
#include <kart/KartDynamics.hpp>
#include <kart/KartWheel.hpp>
#include <kart/KartWheelPhysics.hpp>

nw4r::ut::List Kart::KartObjectProxy::sList;
extern u32 bss_809c190c;
u32 bss_809c190c;

namespace Kart {

void KartObjectProxy::InitList() {
  List_Init(&sList, offsetof(KartObjectProxy, mNode));
}

void KartObjectProxy::SetupInList(KartAccessor* accessor) {
  KartObjectProxy* component = nullptr;
  while ((component = (KartObjectProxy*)List_GetNext(&sList, component))) {
    component->mAccessor = accessor;
  }
}

KartObjectProxy::KartObjectProxy() : mAccessor(nullptr) {
  List_Append(&sList, this);
}

const EGG::Vector3f& KartObjectProxy::getPos() const { return kartDynamics()->pos; }

const EGG::Vector3f& KartObjectProxy::getPrevPos() const { return kartPhysics()->pos; }

void KartObjectProxy::setPos(const EGG::Vector3f &pos) { 
  kartDynamics()->pos = pos;
}

const EGG::Matrix34f& KartObjectProxy::getPose() const { return kartPhysics()->pose; }

const EGG::Matrix34f& KartObjectProxy::getBodyRot() const { return kartBody()->pose; }

void KartObjectProxy::setRot(const EGG::Quatf &rot) {
  KartDynamics* dynamics = kartDynamics();
  dynamics->fullRot = rot;
  dynamics->mainRot = rot;
}

u16 KartObjectProxy::getSuspCount() { return kartSettings()->susCount; }
u16 KartObjectProxy::getWheelCount() { return kartSettings()->wheelCount; }
float KartObjectProxy::getWheelCountRecip() { return kartSettings()->wheelCountRecip; }
float KartObjectProxy::getWheelCountPlusOneRecip() { return kartSettings()->wheelCountPlusOneRecip; }

const EGG::Vector3f& KartObjectProxy::getWheelPos(u32 wheelIdx) {
  return kartWheel(wheelIdx)->getPhysics()->wheelPos;
}

bool KartObjectProxy::wheelIdxHasFloorCollision(u32 wheelIdx) {
  return kartWheel(wheelIdx)->getPhysics()->hasFloorCollision();
}

void KartObjectProxy::setBodyAngle(f32 angle) {
  // TODO
  //kartBody()->setAngle(angle);
}

void KartObjectProxy::setStartBoostIdx(s32 idx) {
  kartState()->setStartBoostIdx(idx);
}

const EGG::Vector3f& KartObjectProxy::getWheelEdgePos(u32 wheelIdx) {
  return kartWheel(wheelIdx)->getPhysics()->wheelEdgePos;
}

KartPhysics* KartObjectProxy::kartPhysics() { return mAccessor->mBody->getPhysics(); }

const KartPhysics* KartObjectProxy::kartPhysics() const { return mAccessor->mBody->getPhysics(); }

KartDynamics* KartObjectProxy::kartDynamics() { return mAccessor->mBody->getPhysics()->mpDynamics; }

const KartDynamics* KartObjectProxy::kartDynamics() const { return mAccessor->mBody->getPhysics()->mpDynamics; }

bool KartObjectProxy::isLocal() {
  return kartState()->on(KART_FLAG_LOCAL);
}

bool KartObjectProxy::isCpu() const {
  return kartState()->on(KART_FLAG_CPU);
}

bool KartObjectProxy::isGhost() {
  return kartState()->on(KART_FLAG_GHOST);
}

KartBody* KartObjectProxy::kartBody() { return mAccessor->mBody; }

const KartBody* KartObjectProxy::kartBody() const { return mAccessor->mBody; }

KartSus* KartObjectProxy::kartSus(s32 idx) { return mAccessor->mSus[idx]; }

const KartSus* KartObjectProxy::kartSus(s32 idx) const {
  return mAccessor->mSus[idx];
}

KartWheel* KartObjectProxy::kartWheel(s32 idx) { return mAccessor->mWheels[idx]; }

const KartWheel* KartObjectProxy::kartWheel(s32 idx) const {
  return mAccessor->mWheels[idx];
}

KartSusPhysics* KartObjectProxy::kartSusPhysics(s32 idx) { return kartSus(idx)->getPhysics(); }

const KartSusPhysics* KartObjectProxy::kartSusPhysics(s32 idx) const {
  return kartSus(idx)->getPhysics();
}

KartWheelPhysics* KartObjectProxy::kartWheelPhysics(s32 idx) { return mAccessor->mWheels[idx]->getPhysics(); }

const KartWheelPhysics* KartObjectProxy::kartWheelPhysics(s32 idx) const {
  return mAccessor->mWheels[idx]->getPhysics();
}


KartPhysicsEngine* KartObjectProxy::kartPhysicsEngine() { return mAccessor->mPhysicsEngine; }

const KartPhysicsEngine* KartObjectProxy::kartPhysicsEngine() const { return mAccessor->mPhysicsEngine; }

KartMove* KartObjectProxy::kartMove() { return mAccessor->mMove; }

const KartMove* KartObjectProxy::kartMove() const { return mAccessor->mMove; }

KartSnd* KartObjectProxy::kartSnd() { return mAccessor->mSnd; }

KartAccessor_34* KartObjectProxy::kartAccessor_34() { return mAccessor->m_34; }

MaybeShadow* KartObjectProxy::maybeShadow() { return mAccessor->mMaybeShadow; }

HitboxGroup* KartObjectProxy::hitboxGroup() { return kartBody()->getPhysics()->mpHitboxGroup; }

const HitboxGroup* KartObjectProxy::hitboxGroup() const { return kartBody()->getPhysics()->mpHitboxGroup; }

const HitboxGroup* KartObjectProxy::wheelHitbox(s32 wheelIdx) const { return kartWheel(wheelIdx)->getPhysics()->getHitbox(); }

const KartCollisionInfo* KartObjectProxy::bodyColInfo() const { return &hitboxGroup()->getKartCollisionInfo(); }

const KartCollisionInfo* KartObjectProxy::wheelColInfo(s32 idx) const { return &kartWheel(idx)->getPhysics()->getKartCollisionInfo(); }

KartCollide* KartObjectProxy::kartCollide() { return mAccessor->mCollide; }

const KartCollide* KartObjectProxy::kartCollide() const {
  return mAccessor->mCollide;
}

KartParam* KartObjectProxy::kartParam() const { return mAccessor->kartSettings->kartParam; }
KartStats* KartObjectProxy::kartStats() const { return mAccessor->kartSettings->kartParam->stats; }
BSP* KartObjectProxy::bsp() const { return mAccessor->kartSettings->kartParam->bsp; }
BspHitbox* KartObjectProxy::bspHitboxes() const { return mAccessor->kartSettings->kartParam->bsp->hitboxes; }
BspWheel* KartObjectProxy::bspWheel(s32 idx) const { return &mAccessor->kartSettings->kartParam->bsp->wheels[idx]; }

s32 KartObjectProxy::getHopStickX() { return kartMove()->hopStickX(); }

s32 KartObjectProxy::getAppliedHopStickX() {
  return kartMove()->getAppliedHopStickX();
}

KartModel* KartObjectProxy::kartModel() { return mAccessor->mModel; }

u8 KartObjectProxy::getPlayerIdx() const { return mAccessor->kartSettings->playerIdx; }

f32 KartObjectProxy::getVehicleSpeed() { return kartMove()->speed(); }

KartAction* KartObjectProxy::kartAction() { return mAccessor->mAction; }

const KartAction* KartObjectProxy::kartAction() const {
  return mAccessor->mAction;
}

void KartObjectProxy::empty(s16 arg) { kartMove()->empty(arg); }

void KartObjectProxy::clearBoost() { kartMove()->clearBoost(); }

f32 KartObjectProxy::getSpeedRatioCapped() {
  return kartMove()->speedRatioCapped();
}

f32 KartObjectProxy::getSpeedRatio() { return kartMove()->speedRatio(); }

bool KartObjectProxy::hasCamera() { return mAccessor->mCamera; }

void KartObjectProxy::setCamera(KartCamera* camera) {
  mAccessor->mCamera = camera;
}

void KartObjectProxy::empty() {}

KartAccessor_54* KartObjectProxy::kartAccessor_54() { return mAccessor->m_54; }

KartBlink* KartObjectProxy::kartBlink() { return mAccessor->mBlink; }

const KartBlink* KartObjectProxy::kartBlink() const {
  return mAccessor->mBlink;
}

KartAccessor_5c* KartObjectProxy::kartAccessor_5c() { return mAccessor->m_5c; }

f32 KartObjectProxy::getBaseSpeed() { return kartMove()->baseSpeed(); }

KartEnemy* KartObjectProxy::kartEnemy() { return mAccessor->mEnemy; }

s16 KartObjectProxy::getDriftState() { return kartMove()->driftState(); }

s16 KartObjectProxy::getMtCharge() { return kartMove()->mtCharge(); }

s16 KartObjectProxy::getSmtCharge() { return kartMove()->smtCharge(); }

s16 KartObjectProxy::getMaxMtCharge() { return kartMove()->getMaxMtCharge(); }

s16 KartObjectProxy::getMaxSmtCharge() { return kartMove()->getMaxSmtCharge(); }

bool KartObjectProxy::isDrivingIdle() {
  return kartMove()->drivingDirection() == KartMove::DRIVING_DIRECTION_IDLE;
}

s16 KartObjectProxy::getBackwardsAllowCounter() {
  return kartMove()->backwardsAllowCounter();
}

KartAccessor_44* KartObjectProxy::kartAccessor_44() { return mAccessor->m_44; }

KartAccessor_48* KartObjectProxy::kartAccessor_48() { return mAccessor->m_48; }

KartNetSender* KartObjectProxy::kartNetSender() { return mAccessor->mSender; }

KartNetReceiver* KartObjectProxy::kartNetReceiver() {
  return mAccessor->mReceiver;
}

void KartObjectProxy::setScale(const EGG::Vector3f& scale) {
  kartMove()->setScale(scale);
}

const EGG::Vector3f& KartObjectProxy::getScale() { return kartMove()->scale(); }

bool KartObjectProxy::inRespawn() {
  return kartMove()->respawnTimer() > 0 || kartMove()->someTimer() > 0;
}

BoxColUnit* KartObjectProxy::kartEntity() { return mAccessor->mEntity; }

KartAccessor_50* KartObjectProxy::kartAccessor_50() { return mAccessor->m_50; }

KartAccessor_60* KartObjectProxy::kartAccessor_60() { return mAccessor->m_60; }

bool KartObjectProxy::isSomeMoveFlagActive() {
  return kartMove()->flags() & 1 << 2;
}

const KartHalfPipe* KartObjectProxy::kartHalfPipe() {
  return kartMove()->kartHalfPipe();
}

const KartJump* KartObjectProxy::kartJump() { return kartMove()->kartJump(); }

} // namespace Kart

extern "C" {

void* PlayerPointers_getVehicle(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x0);
  return *(u8**)(b + 0x4);
}

void* PlayerPointers_isBike(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x0);
  return *(u8**)(b + 0x0);
}

void* PlayerPointers_getRacedataFactoryFlags(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x3C);
  return *(u8**)(b + 0x10);
}

void* PlayerPointers_getOobWipeState(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x4);
  return *(u8**)(b + 0x90);
}

void* various_stuff_get_kart_parts_disp_params(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x0);
  return *(u8**)(b + 0x20);
}

void* various_stuff_get_bike_parts_disp_params(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x0);
  return *(u8**)(b + 0x24);
}

void* PlayerPointers_getKartDriverDispParams(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x0);
  return *(u8**)(b + 0x1C);
}

void* PlayerPointers_getVehicleType(void* self) {
  u8* p = *(u8**)((u8*)self + 0x0);
  p = *(u8**)(p + 0x0);
  p = *(u8**)(p + 0x14);
  p = *(u8**)(p + 0x0);
  return *(u8**)(p + 0x4);
}

void* PlayerPointers_getMalCol2(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x8);
  u8* c = *(u8**)(b + 0x90);
  return c + 0xE4;
}

void* PlayerPointers_getPlayerPhysicsHolderPosition(void* self) {
  u8* a = *(u8**)((u8*)self + 0x0);
  u8* b = *(u8**)(a + 0x8);
  u8* c = *(u8**)(b + 0x90);
  return c + 0x18;
}

void fn_1_800C0() {}
void fn_1_801B8() {}
void fn_1_80298() {}
void fn_1_80A88() {}
void fn_1_80AA0() {}
void fn_1_80AB8() {}
void fn_1_80AD0() {}
void fn_1_80AEC() {}
void fn_1_80BB8() {}
void fn_1_813E8() {}
void fn_1_81494() {}

}
