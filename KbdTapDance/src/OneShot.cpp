/*
 * arduino-kbd-tap-dance -- multi-use keys for arduino keyboards
 * Copyright (C) 2016  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OneShot.h"

OneShotKey::OneShotKey (uint8_t code) : OneShotKey (code, ONESHOT_TIMEOUT_DEFAULT) {
}

OneShotKey::OneShotKey (uint8_t code, uint16_t timeout) : BasicKey (code) {
  timer = Timer (timeout);
  _reset ();
}

bool
OneShotKey::shouldIgnore (uint8_t) {
  return false;
}

void
OneShotKey::_reset (void) {
  timer = 0;

  sticky = false;
  active = false;
  pressed = false;
  cancel = false;
}

bool
OneShotKey::press (uint8_t code) {
  if (code != this->keycode) {
    if (this->sticky)
      return true;

    if (this->active) {
      if (this->pressed)
        this->cancel = !this->shouldIgnore (code);
      else
        this->active = this->shouldIgnore (code);
    }

    return true;
  }

  this->pressed = true;
  if (this->active) {
    if (this->sticky) {
      this->onDeactivate ();
      _reset ();
      return false;
    }

    this->sticky = true;
    return false;
  }

  this->active = true;
  this->onActivate ();
  return false;
}

bool
OneShotKey::release (uint8_t code) {
  if (!this->active)
    return (this->keycode != code);

  if (this->keycode == code)
    this->pressed = false;

  if (this->cancel && code == this->keycode) {
    this->onDeactivate ();
    _reset();
  }

  return (this->keycode != code);
}

void
OneShotKey::cycle (void) {
  if (!this->active && !(this->timer == 0)) {
    this->onDeactivate ();
    _reset ();
    return;
  }

  timer++;

  if (timer.timedout ()) {
    if (!this->pressed) {
      this->onDeactivate ();
      _reset ();
    } else {
      this->cancel = true;
    }
  }
}

void
OneShotModifierKey::onActivate (void) {
  this->register_code (this->kc);
}

void
OneShotModifierKey::onDeactivate (void) {
  this->unregister_code (this->kc);
}
