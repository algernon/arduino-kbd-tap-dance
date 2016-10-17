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

class BasicKey {
 protected:
  uint8_t index;

 public:
  BasicKey (uint8_t index);
  virtual ~BasicKey (void);

  virtual bool press (uint8_t index) = 0;
  virtual bool press (void);
  virtual bool release (uint8_t index) = 0;
  virtual bool release (void);
  virtual void cycle (void) {};

  uint8_t get_index (void) const;
};

class HWKey {
 public:
  HWKey (void) {};
  virtual ~HWKey (void) {};

  virtual void register_code (uint8_t code) = 0;
  virtual void unregister_code (uint8_t code) = 0;
};

class SelfKey : public BasicKey {
 protected:
  uint8_t keycode;
  HWKey *hw;

 public:
  SelfKey (HWKey *hw, uint8_t index, uint8_t keycode);

  virtual bool press (uint8_t index);
  virtual bool release (uint8_t index);

  uint8_t get_code (void) const;
};

class ModdedKey : public SelfKey {
 protected:
  SelfKey *k;

 public:
  ModdedKey (HWKey *hw, uint8_t index, uint8_t mod, SelfKey *k);
  ~ModdedKey (void);

  virtual bool press (uint8_t index);
  virtual bool release (uint8_t index);

  SelfKey *get_key (void) const;
};
