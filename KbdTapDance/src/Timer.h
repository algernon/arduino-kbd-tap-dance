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

#ifndef _KBDHACKS_TIMEOUT_H
#define _KBDHACKS_TIMEOUT_H 1

#include <stdint.h>

#define TIMEOUT_DEFAULT 40

class Timer {
 protected:
  uint16_t _timer;
  uint16_t _timeout;

 public:
  Timer (void);
  Timer (uint16_t timeout);

  uint16_t operator++ (int);

  bool operator== (const uint16_t rhs);

  uint16_t operator() () const;
  void operator() (const uint16_t newTimeout);

  Timer& operator= (uint16_t newTimer);

  bool timedout (void);
};

#endif
