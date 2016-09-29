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

#ifndef _KBDHACKS_ONESHOT_H
#define _KBDHACKS_ONESHOT_H 1

#include <stdint.h>

#define ONESHOT_TIMEOUT_DEFAULT 40

class OneShotKey {
 private:
  void _reset (void);

 protected:
  uint16_t timer;
  uint16_t shot_timeout;
  uint8_t keycode;
  bool active;
  bool sticky;
  bool pressed;
  bool cancel;

  virtual void register_code (uint8_t code) = 0;
  virtual void unregister_code (uint8_t code) = 0;

  virtual void onActivate (void) {};
  virtual void onDeactivate (void) {};

  virtual bool shouldIgnore (uint8_t code);

 public:
  OneShotKey (uint8_t code);
  OneShotKey (uint8_t code, uint16_t timeout);

  void press (uint8_t code);
  void release (uint8_t code);
  void cycle (void);

  uint16_t timeout (void);
  void timeout (uint16_t new_timeout);
};

#endif
