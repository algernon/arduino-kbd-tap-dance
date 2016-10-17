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
  TapDanceKey (uint8_t index);
  TapDanceKey (uint8_t index, uint16_t timeout);

  virtual bool press (uint8_t index);
  virtual bool release (uint8_t index);
  virtual void cycle (void);
};

class TapDanceDoubleKey : public TapDanceKey {
 protected:
  BasicKey *key1, *key2;

  virtual void onFinish (void);
  virtual void onReset (void);

 public:
  TapDanceDoubleKey (uint8_t index, BasicKey *key1, BasicKey *key2);
  ~TapDanceDoubleKey (void);
};
