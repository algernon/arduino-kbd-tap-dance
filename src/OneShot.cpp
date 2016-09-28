#include "OneShot.h"

OneShotKey::OneShotKey (uint8_t code) {
  keycode = code;
  _reset ();
}

void
OneShotKey::_reset (void) {
  sticky = false;
  active = false;
  timer = 0;
}

void
OneShotKey::press (uint8_t code) {
  if (code != this->keycode) {
    if (this->sticky)
      return;

    if (this->active)
      this->active = false;

    return;
  }

  if (this->active) {
    if (this->sticky) {
      this->unregister_code (this->keycode);
      this->onDeactivate ();
      _reset ();
      return;
    }

    this->sticky = true;
    return;
  }

  this->active = true;

  this->register_code (this->keycode);
  this->onActivate ();
}

void
OneShotKey::release (uint8_t code) {
}

void
OneShotKey::cycle (void) {
  if (!this->active && this->timer) {
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
