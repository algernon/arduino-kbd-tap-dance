#include "OneShot.h"

OneShotKey::OneShotKey (uint8_t code) {
  keycode = code;
  shot_timeout = ONESHOT_TIMEOUT_DEFAULT;
  _reset ();
}

OneShotKey::OneShotKey (uint8_t code, uint16_t timeout) : OneShotKey (code) {
  this->timeout (timeout);
}

bool
OneShotKey::shouldIgnore (uint8_t code) {
  return false;
}

void
OneShotKey::timeout (uint16_t new_timeout) {
  this->shot_timeout = new_timeout;
}

uint16_t
OneShotKey::timeout (void) {
  return this->shot_timeout;
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
        this->cancel = !this->shouldIgnore (code);
      else
        this->active = this->shouldIgnore (code);
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

  if (timer < this->shot_timeout)
    timer++;

  if (timer == this->shot_timeout) {
    if (!this->pressed) {
      this->unregister_code (this->keycode);
      this->onDeactivate ();
      _reset ();
    } else {
      this->cancel = true;
    }
  }
}
