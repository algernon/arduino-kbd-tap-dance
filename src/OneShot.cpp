#include "OneShot.h"

OneShotKey::OneShotKey (uint8_t code) {
  keycode = code;
  _reset ();
}

void
OneShotKey::_reset (void) {
}

void
OneShotKey::press (uint8_t code) {
  if (code == this->keycode) {
    this->register_code (this->keycode);
    this->onActivate ();
  }
}

void
OneShotKey::release (uint8_t code) {
}

void
OneShotKey::cycle (void) {
  timer++;
  if (timer == 40) {
    this->unregister_code (this->keycode);
    this->onDeactivate ();
  }
}
