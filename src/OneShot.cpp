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
      this->onDeactivate ();
      _reset ();
      return;
    }

    this->sticky = true;
    return;
  }

  this->active = true;
  this->onActivate ();
}

void
OneShotKey::release (uint8_t code) {
  if (!this->active)
    return;

  if (this->keycode == code)
    this->pressed = false;

  if (this->cancel && code == this->keycode) {
    this->onDeactivate ();
    _reset();
  }
}

void
OneShotKey::cycle (void) {
  if (!this->active && this->timer) {
    this->onDeactivate ();
    _reset ();
    return;
  }

  if (timer < this->shot_timeout)
    timer++;

  if (timer == this->shot_timeout) {
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
  this->register_code (this->keycode);
}

void
OneShotModifierKey::onDeactivate (void) {
  this->unregister_code (this->keycode);
}
