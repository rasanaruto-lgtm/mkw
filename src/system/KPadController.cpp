#include "KPadController.hpp"

#include <string.h>

#include <system/KPadDirector.hpp>

namespace System {
float rawStickToFloat(u8 stick);

void KPadRaceInputState::reset() {
  mButtons = 0;
  mButtonsRaw = 0;
  mStick.y = 0.f;
  mStick.x = 0.f;
  mStickXRaw = 7;
  mStickYRaw = 7;
  mTrick = 0;
  mTrickRaw = 0;
  mIsValid = 0;
}

bool KPadRaceInputState::operator==(const KPadRaceInputState& rhs) const {
  if (!mIsValid || !rhs.mIsValid) {
      return false;
  }

  f32 rx = rhs.mStick.x; 
  f32 lx = mStick.x;
  if (lx != rx) {
    return false;
  }

  f32 ry = rhs.mStick.y;
  f32 ly = mStick.y;
  if (ly != ry) {
    return false;
  }

  return mButtons == rhs.mButtons;
}

void KPadUIInputState::reset() {
  mButtons = 0;  
  mButtonsRaw = 0;
  mStick.y = 0.0f;
  mStick.x = 0.0f;
  mStickXRaw = 7;
  mStickYRaw = 7;
  mFlags_0 = 0;
  mPointerPos.x = -1.f;
  mPointerPos.y = -1.f;
  mPointerHorizontal.y = 0.f;
  mPointerHorizontal.x = 0.f;
  mPointerDist = 0.f;
  mFlags_1 = 0;
}

void KPadRaceInputState::setStickXMirrored(u8 x) {
  mStickXRaw = x;
  mStick.x = KPadDirector::spInstance->mIsMirror ?
              -rawStickToFloat(x) : rawStickToFloat(x);
}

void KPadRaceInputState::setStickXUnmirrored(u8 x) {
  mStickXRaw = x;
  mStick.x = rawStickToFloat(x);
}

float rawStickToFloat(u8 stick) {
  return (stick - 7.f) / 7.f;
}

void KPadRaceInputState::setStickY(u8 y) {
  mStickYRaw = y;
  mStick.y = rawStickToFloat(y);
}

void KPadUIInputState::setStickXMirrored(u8 x) {
  mStickXRaw = x;
  mStick.x = KPadDirector::spInstance->mIsMirror ?
              -rawStickToFloat(x) : rawStickToFloat(x);
}

void KPadUIInputState::setStickY(u8 y) {
  mStickYRaw = y;
  mStick.y = rawStickToFloat(y);
}

void KPadRaceInputState::setTrick(eTrick trick) {
  mTrickRaw = trick;
  eTrick actualTrick = trick;
  if (KPadDirector::spInstance->mIsMirror) {
    switch (trick) {
    case LEFT_TRICK:
      actualTrick = RIGHT_TRICK;
      break;
    case RIGHT_TRICK:
      actualTrick = LEFT_TRICK;
      break;
    default:
      break;
    }
  }
  mTrick = actualTrick;
}

void KPadController::setDriftIsAuto(bool isAuto) {
  mDriftIsAuto = isAuto;
}

void KPadController::setDpdEnabled(bool enabled) {}

void KPadController::startMotor() {}

void KPadController::stopMotor() {}

u32 KPadController::getBatteryLevelAsync() { return 2; }

int KPadController::getChan() { return -1; }

bool KPadController::isRumbleEnabled() { return false; }

bool KPadController::isDpdEnabled() { return false; }

int KPadGCController::getChan() { return mChan; }

int KPadWiiController::getChan() { return mChan; }

eControlSource KPadWiiController::getControlSource() { return mControlSource; }

eControlSource KPadGhostController::getControlSource() {
  return CONTROL_SOURCE_GAMECUBE;
}

u16* KPadPlayer::getRecordedBuffer() { return mRecordedBuffer; }

u32 KPadPlayer::getRecordedBufferSize() { return mRecordedBufferSize; }

void KPadGCController::startMotor() { PADControlMotor(mChan, 1); }

void KPadGCController::stopMotor() { PADControlMotor(mChan, 0); }

void KPadGCController::stopMotorHard() { PADControlMotor(mChan, 2); }

void KPadWiiController::startMotor() { WPADControlMotor(mChan, 1); }

void KPadWiiController::stopMotor() { WPADControlMotor(mChan, 0); }

void KPadWiiController::stopMotorHard() { stopMotor(); }

bool KPadWiiController::isRumbleEnabled() { return mType == 2; }

bool KPadGhostController::hasGhostBuffer() { return mGhostBuffer != nullptr; }

void KPadGhostController::setDriftIsAuto(bool isAuto) {}

void KPad::startMotor() { mController->startMotor(); }

void KPad::stopMotor() { mController->stopMotor(); }

void KPad::stopMotorHard() { mController->stopMotorHard(); }

void KPad::setInputState(KPadRaceInputState* raceInputState) {}

void KPadController::reset(bool driftIsAuto) {
  setDriftIsAuto(driftIsAuto);
  mRaceInputState.reset();
}

void KPadGCController::reset(bool driftIsAuto) {
  setDriftIsAuto(driftIsAuto);
  mRaceInputState.reset();
  memset(&mPadStatus, 0, 0xC);
  _a8 = 0;
  _aa = false;
  _ac = 0;
}

extern "C" void fn_1_235630(void*, u8);

void KPadWiiController::reset(bool driftIsAuto) {
  setDriftIsAuto(driftIsAuto);
  mRaceInputState.reset();
  memset(&mKpadStatuses[0], 0, 0x840);
  mType = 4;
  mControlSource = (eControlSource)-1;
  _8f8 = 0;
  _8fa = 0;
  mClassicIsInDrift = false;
  _8fc = 0;
  _906 = 4;
  mPointerPos.x = 0.0f;
  mPointerPos.y = 0.0f;
  mPointerHorizontal.y = 0.0f;
  mPointerHorizontal.x = 0.0f;
  mPointerDist = 0.0f;
  fn_1_235630(mWiiControllerData, mDriftIsAuto);
}

bool KPadControllerInfo::Addr::operator==(const Addr& rhs) const {
  for (int i = 0; i < 6; i++) {
    if (mBytes[i] != rhs.mBytes[i]) {
      return false;
    }
  }
  return true;
}

void KPadWiiController::raceToUIInputFreestyle(KPadUIInputState* dst,
                                               KPadRaceInputState* src) {
  u16 buttons = src->mButtonsRaw;
  u16 out = 0;
  if (buttons & 0x900)
    out |= 0x1;
  if (buttons & 0x600)
    out |= 0x2;
  if (buttons & 0x10)
    out |= 0x4;
  if (buttons & 0x8)
    out |= 0x8;
  if (buttons & 0x4)
    out |= 0x10;
  if (buttons & 0x1)
    out |= 0x20;
  if (buttons & 0x2)
    out |= 0x40;
  if (buttons & 0x8000)
    out |= 0x80;
  if (buttons & 0x5000)
    out |= 0x100;
  dst->mButtons = out;
  dst->mButtonsRaw = src->mButtonsRaw;
  u8 xRaw = src->mStickXRaw;
  dst->mStickXRaw = xRaw;
  dst->mStick.x = KPadDirector::spInstance->mIsMirror ? -rawStickToFloat(xRaw)
                                                      : rawStickToFloat(xRaw);
  u8 yRaw = src->mStickYRaw;
  dst->mStickYRaw = yRaw;
  dst->mStick.y = rawStickToFloat(yRaw);
}

void KPadWiiController::raceToUIInputCore(KPadUIInputState* dst,
                                          KPadRaceInputState* src) {
  u16 buttons = src->mButtonsRaw;
  u16 out = 0;
  if (buttons & 0x900)
    out |= 0x1;
  if (buttons & 0x600)
    out |= 0x2;
  if (buttons & 0x10)
    out |= 0x4;
  if (buttons & 0x2)
    out |= 0x8;
  if (buttons & 0x1)
    out |= 0x10;
  if (buttons & 0x8)
    out |= 0x20;
  if (buttons & 0x4)
    out |= 0x40;
  if (buttons & 0x8000)
    out |= 0x80;
  if (buttons & 0x1000)
    out |= 0x100;
  dst->mButtons = out;
  dst->mButtonsRaw = src->mButtonsRaw;
}

f32 KPadController::getStreamSize() { return 0.0f; }

u32 KPadFaceButtonsStream::getSequenceSize() { return 2; }

u32 KPadTrickButtonsStream::getSequenceSize() { return 2; }

u32 KPadDirectionButtonsStream::getSequenceSize() { return 2; }

KPadFaceButtonsStream::~KPadFaceButtonsStream() {}

KPadTrickButtonsStream::~KPadTrickButtonsStream() {}

KPadDirectionButtonsStream::~KPadDirectionButtonsStream() {}

void KPadWiiController::setDpdEnabled(bool enabled) {
  if (enabled)
    KPADEnableDPD(mChan);
  else
    KPADDisableDPD(mChan);
}

void KPadWiiController::updateBattery(int result) {
  mOutOfBattery = false;
  if (result == 0) {
    mBattery = mWpadInfo.battery;
  }
}
} // namespace System
