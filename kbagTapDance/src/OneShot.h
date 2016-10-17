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

#define ONESHOT_TIMEOUT_DEFAULT TIMEOUT_DEFAULT

class OneShotKey : public BasicKey {
 private:
  void _reset (void);

 protected:
  Timer timer;
  bool active;
  bool sticky;
  bool pressed;
  bool cancel;

  virtual void onActivate (void) {};
  virtual void onDeactivate (void) {};

  virtual bool shouldIgnore (uint8_t index);

 public:
  OneShotKey (uint8_t index);
  OneShotKey (uint8_t index, uint16_t timeout);

  virtual bool press (uint8_t index);
  virtual bool release (uint8_t index);
  virtual void cycle (void);
};

class OneShotModifierKey : public OneShotKey {
 protected:
  BasicKey *mod;

  virtual void onActivate (void);
  virtual void onDeactivate (void);

 public:
  OneShotModifierKey (uint8_t index, BasicKey *mod);
  OneShotModifierKey (uint8_t index, BasicKey *mod, uint16_t timeout);
  virtual ~OneShotModifierKey (void);
};
