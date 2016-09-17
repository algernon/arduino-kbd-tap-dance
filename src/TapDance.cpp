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
  interrupted = false;
  timedout = false;
}

void
TapDanceKey::press (uint8_t code) {
  if (this->keycode == code) {
    this->count++;
    this->pressed = true;

    this->onEachTap ();
    return;
  }

  this->interrupted = true;
  this->onFinish ();

  if (!this->pressed || !this->timedout) {
    this->onReset ();

    _reset ();
  }
}

void
TapDanceKey::release (uint8_t code) {
  this->pressed = false;
  if (this->timedout || this->interrupted) {
    this->onReset ();
    _reset();
  }
}

void
TapDanceKey::cycle (void) {
  timer++;
  if (timer == 40) {
    timedout = true;
    this->onFinish ();

    if (!this->pressed) {
      this->onReset ();

      _reset ();
    }
  }
}
