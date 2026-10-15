#include "KartSus.hpp"
#include "KartWheelPhysics.hpp"

extern "C" {

void KartSus_initPhysics(void* self) {
  ((Kart::KartSusPhysics*)*(u8**)((u8*)self + 0x90))->init();
}

void fn_1_88828(void* self) {
  ((Kart::KartSusPhysics*)*(u8**)((u8*)self + 0x90))->reset();
}

void tire0_set_initial_physics_values(void* self) {
  ((Kart::KartSusPhysics*)*(u8**)((u8*)self + 0x90))->setInitialState();
}

}
