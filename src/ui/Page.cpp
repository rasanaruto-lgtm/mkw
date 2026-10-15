#include "Page.hpp"
#include "ControlGroup.hpp"

namespace UI {

void Page::draw(u32 pass) {
    if (mState > 5 || mState < 3) {
        return;
    }

    mControlGroup.draw(pass);
}

void Page::setAnimationDirection(s32 animationDirection) {
    if (animationDirection == -1) {
        return;
    }

    mAnimationDirection = animationDirection;
}

void Page::initChildren(s32 capacity) {
    mControlGroup.create(this, capacity);
}

void Page::insert(s32 index, UIControl* control, u32 drawPass) {
    mControlGroup.insert(index, control, drawPass);
}

void Page::setInputManager(MenuInputManager* inputManager) {
    mInputManager = inputManager;
}

void Page::skipInAnimation(void) {
    mNextStateRequested = true;
}

void Page::startReplace(s32 animationDirection, f32 delay) {
  setAnimationDirection(animationDirection);
  mAnimationDelay = delay;
  mNextStateRequested = true;
}

void Page::skipOutAnimation(void) {
    mNextStateRequested = true;
}

}

extern "C" {

void Page_onInit() {}
void fn_1_F1FA4() {}
void fn_1_F1FD8() {}
void fn_1_F2094() {}

void fn_1_F296C(void* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
}

}
