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
  pressed = false;
  cancel = false;
}

void
OneShotKey::press (uint8_t code) {
  if (code != this->keycode) {
    if (this->sticky)
      return;

    if (this->active) {
      if (this->pressed)
        this->cancel = true;
      else
        this->active = false;
    }

    return;
  }

  this->pressed = true;
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
  if (!this->active)
    return;

  if (this->keycode == code)
    this->pressed = false;

  if (this->cancel && code == this->keycode) {
    this->unregister_code (this->keycode);
    this->onDeactivate ();
    _reset();
  }
}

void
OneShotKey::cycle (void) {
  if (!this->active && this->timer) {
    this->unregister_code (this->keycode);
    this->onDeactivate ();
    _reset ();
    return;
  }

  if (timer < 40)
    timer++;

  if (timer == 40) {
    if (!this->pressed) {
      this->unregister_code (this->keycode);
      this->onDeactivate ();
      _reset ();
    } else {
      this->cancel = true;
    }
  }
}
