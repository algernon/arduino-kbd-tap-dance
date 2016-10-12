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

#include "TapDance.h"

TapDanceKey::TapDanceKey (uint8_t code) : TapDanceKey (code, TAP_DANCE_TIMEOUT_DEFAULT) {
}

TapDanceKey::TapDanceKey (uint8_t code, uint16_t timeout) {
  keycode = code;
  timer = Timer (timeout);
  _reset ();
}

void
TapDanceKey::_reset (void) {
  timer.reset ();

  count = 0;
  pressed = false;
  interrupted = false;
  timedout = false;
}

bool
TapDanceKey::press (uint8_t code) {
  if (this->keycode == code) {
    this->count++;
    this->pressed = true;

    this->onEachTap ();
    return false;
  }

  if (this->count == 0)
    return true;

  this->interrupted = true;
  this->onFinish ();

  if (!this->pressed || !this->timedout) {
    this->onReset ();

    _reset ();
  }

  return true;
}

bool
TapDanceKey::release (uint8_t code) {
  if (code != this->keycode)
    return true;

  this->pressed = false;
  if (this->timedout || this->interrupted) {
    this->onReset ();
    _reset();
  }
  return false;
}

void
TapDanceKey::cycle (void) {
  timer++;

  if (timer.timedout () && !timedout) {
    timedout = true;
    this->onFinish ();

    if (!this->pressed) {
      this->onReset ();

      _reset ();
    }
  }
}

// --- Double ---
void
TapDanceDoubleKey::onFinish () {
  uint8_t kc = this->kc1;
  if (this->count >= 2)
    kc = this->kc2;

  this->register_code (kc);
}

void
TapDanceDoubleKey::onReset () {
  uint8_t kc = this->kc1;
  if (this->count >= 2)
    kc = this->kc2;

  this->unregister_code (kc);
}
