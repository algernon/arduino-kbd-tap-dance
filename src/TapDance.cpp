#include "TapDance.h"

TapDanceKey::TapDanceKey (uint8_t code) {
  keycode = code;
  _reset ();
}

void
TapDanceKey::_reset (void) {
  count = 0;
  timer = 0;
  pressed = false;
  finished = false;
  interrupted = false;
}

void
TapDanceKey::press (uint8_t code) {
  this->count++;
  this->pressed = true;

  this->onEachTap ();
}

void
TapDanceKey::release (uint8_t code) {
  this->pressed = false;
}

void
TapDanceKey::cycle (void) {
  timer++;
  if (timer == 40) {
    finished = true;
    this->onFinish ();
    this->onReset ();

    _reset ();
  }
}
