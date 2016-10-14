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

#ifndef _KBDHACKS_TAPDANCE_H
#define _KBDHACKS_TAPDANCE_H 1

#include <stdint.h>
#include "BasicKey.h"
#include "Timer.h"

#define TAP_DANCE_TIMEOUT_DEFAULT TIMEOUT_DEFAULT

class TapDanceKey : public BasicKey {
 private:
  void _reset (void);

 protected:
  Timer timer;
  uint8_t count;
  bool pressed;
  bool interrupted;
  bool timedout;

  virtual void onFinish (void) {};
  virtual void onReset (void) {};
  virtual void onEachTap (void) {};

 public:
  TapDanceKey (uint8_t code);
  TapDanceKey (uint8_t code, uint16_t timeout);

  virtual bool press (uint8_t code);
  virtual bool release (uint8_t code);
  virtual void cycle (void);
};

class TapDanceDoubleKey : public TapDanceKey {
 private:
  uint8_t kc1, kc2;

 protected:
  virtual void register_code (uint8_t code) = 0;
  virtual void unregister_code (uint8_t code) = 0;

  virtual void onFinish (void);
  virtual void onReset (void);

 public:
  TapDanceDoubleKey (uint8_t code, uint8_t kc1, uint8_t kc2) : TapDanceKey (code) {
    this->kc1 = kc1;
    this->kc2 = kc2;
  };
};

#endif
