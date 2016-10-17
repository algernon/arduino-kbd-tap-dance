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

#include "kbagLayout.h"

Layout::Layout (HWKey *hw) {
  this->hw = hw;
}

Layout::Layout (HWKey *hw, uint16_t layout[LAYOUT_SIZE]) : Layout (hw) {
  setup (layout);
}

void
Layout::setup (uint16_t layout[LAYOUT_SIZE]) {
  for (uint8_t i = 0; i < LAYOUT_SIZE; i++) {
    this->layout[i] = this->createKey (i, layout[i]);
  }
}

Layout::~Layout () {
  for (uint8_t i = 0; i < LAYOUT_SIZE; i++) {
    delete (this->layout[i]);
  }
  delete (this->hw);
}

BasicKey *
Layout::createKey (uint8_t index, uint16_t code) {
  uint16_t mods = (code >> 8) << 8;
  uint8_t kc = code - mods;
  SelfKey *k = new SelfKey (hw, index, kc);

  if (mods & MOD_LCTL) {
    k = new ModdedKey (hw, index, KC_LCTL, k);
  }
  if (mods & MOD_LSFT) {
    k = new ModdedKey (hw, index, KC_LSFT, k);
  }
  if (mods & MOD_LALT) {
    k = new ModdedKey (hw, index, KC_LALT, k);
  }
  if (mods & MOD_LGUI) {
    k = new ModdedKey (hw, index, KC_LGUI, k);
  }

  if (mods & MOD_RCTL) {
    k = new ModdedKey (hw, index, KC_RCTL, k);
  }
  if (mods & MOD_RSFT) {
    k = new ModdedKey (hw, index, KC_RSFT, k);
  }
  if (mods & MOD_RALT) {
    k = new ModdedKey (hw, index, KC_RALT, k);
  }

  return k;
}

void
Layout::press (uint8_t index) {
  for (uint8_t i = 0; i < LAYOUT_SIZE; i++) {
    layout[i]->press (index);
  }
}

void
Layout::release (uint8_t index) {
  for (uint8_t i = 0; i < LAYOUT_SIZE; i++) {
    layout[i]->release (index);
  }
}

void
Layout::cycle (void) {
  for (uint8_t i = 0; i < LAYOUT_SIZE; i++) {
    layout[i]->cycle ();
  }
}
