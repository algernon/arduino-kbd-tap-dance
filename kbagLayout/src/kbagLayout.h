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

#pragma once

#include "BasicKey.h"
#include "TapDance.h"
#include "OneShot.h"
#include "Leader.h"

#define LAYOUT_SIZE 64
#define KO_NO NULL

enum {
  KC_NO = 0,
  KC_A = 0x04,
  KC_B,
  KC_C,
  KC_D,
  KC_E,
  KC_F,
  KC_G,
  KC_H,
  KC_I,
  KC_J,
  KC_K,
  KC_L,
  KC_M,
  KC_N,
  KC_O,
  KC_P,
  KC_Q,
  KC_R,
  KC_S,
  KC_T,
  KC_U,
  KC_V,
  KC_W,
  KC_X,
  KC_Y,
  KC_Z,

  KC_1 = 0x1E,
  KC_2,
  KC_3,
  KC_4,
  KC_5,
  KC_6,
  KC_7,
  KC_8,
  KC_9,
  KC_0,

  KC_ENT,
  KC_ESC,
  KC_BSPC,
  KC_TAB,
  KC_SPC,
  KC_MINS,
  KC_EQL,
  KC_LBRC,
  KC_RBRC,
  KC_BSLS,

  KC_QUOT = 0x034,
  KC_GRV,
  KC_COMM,
  KC_DOT,
  KC_SLSH,

  KC_F11 = 0x44,

  KC_MPLY = 0xB0, // TODO
  KC_MSTP = 0xB7,

  KC_LCTL = 0xE0,
  KC_LSFT,
  KC_LALT,
  KC_LGUI,
  KC_RCTL,
  KC_RSFT,
  KC_RALT,
  KC_RGUI
};

enum {
  MOD_LCTL = 0x0100,
  MOD_LSFT = 0x0200,
  MOD_LALT = 0x0400,
  MOD_LGUI = 0x0800,

  MOD_RCTL = 0x1000,
  MOD_RSFT = 0x2000,
  MOD_RALT = 0x4000,

  MOD_FN   = 0x8000
};

#define LCTL(kc) (kc | MOD_LCTL)
#define LSFT(kc) (kc | MOD_LSFT)
#define LALT(kc) (kc | MOD_LALT)
#define LGUI(kc) (kc | MOD_LGUI)
#define RCTL(kc) (kc | MOD_RCTL)
#define RSFT(kc) (kc | MOD_RSFT)
#define RALT(kc) (kc | MOD_RALT)
#define FN(kc)   (kc | MOD_FN)

class Layout {
 protected:
  BasicKey *layout[LAYOUT_SIZE];
  HWKey *hw;
 public:
  Layout (HWKey *hw);
  Layout (HWKey *hw, uint16_t layout[LAYOUT_SIZE]);
  virtual ~Layout (void);

  virtual void setup (uint16_t layout[LAYOUT_SIZE]);
  virtual BasicKey *createKey (uint8_t index, uint16_t code);

  virtual void press (uint8_t index);
  virtual void release (uint8_t index);
  virtual void cycle (void);
};
