// -*- mode: c++ -*-

#include "BasicKey.h"
#include "Timer.h"
#include "TapDance.h"
#include "OneShot.h"
#include "Leader.h"

#include "kbagLayout.h"

Layout *layout;

static uint16_t TestLayout[] = {
   KC_F11   ,KC_9     ,KC_7     ,KC_5     ,KC_3     ,KC_1     ,KC_NO
  ,KC_BSLS  ,KC_X     ,KC_W     ,KC_C     ,KC_H     ,KC_F
  ,KC_TAB   ,KC_A     ,KC_O     ,KC_E     ,KC_I     ,KC_U     ,LSFT(KC_9)
  ,KC_MPLY  ,KC_Z     ,KC_Q     ,KC_QUOT  ,KC_COMM  ,KC_DOT   ,KC_NO

                                            ,KC_NO    ,KC_BSPC  ,KC_NO    ,KC_NO
                                                          ,KC_NO


                                                    ,KC_NO    ,KC_0     ,KC_2     ,KC_4     ,KC_6     ,KC_8     ,KC_NO
                                                              ,KC_M     ,KC_G     ,KC_L     ,KC_P     ,KC_SLSH  ,KC_GRV
                                                    ,KC_NO    ,KC_D     ,KC_R     ,KC_T     ,KC_N     ,KC_S     ,KC_EQL
                                                    ,KC_NO    ,KC_B     ,KC_K     ,KC_V     ,KC_Y     ,KC_J     ,KC_MSTP

                                            ,KC_NO    ,KC_SPC   ,KC_ENT   ,KC_MINS
                                                          ,KC_NO
};

class DummyHW : public HWKey {
public:
  DummyHW (void) {};

  void register_code (uint8_t) {};
  void unregister_code (uint8_t) {};
};

void setup() {
  layout = new Layout (new DummyHW (), TestLayout);
}

void loop() {
  for (uint8_t i = 0; i < LAYOUT_SIZE; i++) {
    layout->press (i);
    layout->cycle ();
    layout->release (i);
  }
}
