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

#include "BasicKey.h"

BasicKey::BasicKey (uint8_t index) {
  this->index = index;
}

BasicKey::~BasicKey (void) {
}

bool
BasicKey::press (void) {
  return press (index);
}

bool
BasicKey::release (void) {
  return release (index);
}

uint8_t
BasicKey::get_index (void) const {
  return this->index;
}

// SelfKey

SelfKey::SelfKey (HWKey *hw, uint8_t index, uint8_t keycode) : BasicKey (index) {
  this->keycode = keycode;
  this->hw = hw;
}

bool
SelfKey::press (uint8_t) {
  hw->register_code (keycode);
  return true;
}

bool
SelfKey::release (uint8_t) {
  hw->unregister_code (keycode);
  return true;
}

uint8_t
SelfKey::get_code (void) const {
  return this->keycode;
}

// ModdedKey

ModdedKey::ModdedKey (HWKey *hw, uint8_t index, uint8_t mod, SelfKey *k) : SelfKey (hw, index, mod) {
  this->k = k;
}

ModdedKey::~ModdedKey (void) {
  delete (this->k);
}

bool
ModdedKey::press (uint8_t index) {
  return SelfKey::press (index) && this->k->press (index);
}

bool
ModdedKey::release (uint8_t index) {
  return this->k->release (index) && SelfKey::release (index);
}

SelfKey *
ModdedKey::get_key (void) const {
  return this->k;
}
