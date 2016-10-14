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

#ifndef _KBDHACKS_BASICKEY_H
#define _KBDHACKS_BASICKEY_H 1

#include <stdint.h>

class BasicKey {
 protected:
  uint8_t keycode;

 public:
  BasicKey (uint8_t code);

  virtual bool press (uint8_t code) = 0;
  virtual bool release (uint8_t code) = 0;
  virtual void cycle (void) = 0;
};

#endif
