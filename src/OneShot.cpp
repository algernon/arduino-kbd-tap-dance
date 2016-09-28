#include "OneShot.h"

OneShotKey::OneShotKey (uint8_t code) {
  keycode = code;
  _reset ();
}

void
OneShotKey::_reset (void) {
  cancelled = false;
  timer = 0;
  count = 0;
}

void
OneShotKey::press (uint8_t code) {
  if (code == this->keycode) {
    this->count++;
    this->register_code (this->keycode);
    this->onActivate ();
    return;
  }

  if (this->count > 0)
    this->cancelled = true;
}

void
OneShotKey::release (uint8_t code) {
}

void
OneShotKey::cycle (void) {
  if (this->cancelled) {
    this->unregister_code (this->keycode);
    this->onDeactivate ();
    _reset ();
    return;
  }

  timer++;
  if (timer == 40) {
    this->unregister_code (this->keycode);
    this->onDeactivate ();
  }
}
