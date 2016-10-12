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

#include "TimeOut.h"

Timer::Timer (uint16_t timeout) {
  _timeout = timeout;
  reset ();
}

Timer::Timer (void) : Timer (TIMEOUT_DEFAULT) {
}

void
Timer::reset (void) {
  _timer = 0;
}

uint16_t
Timer::operator++ (int) {
  if (!timedout ())
    return _timer++;
  else
    return _timer;
}

bool
Timer::operator== (uint16_t rhs) {
  return _timer == rhs;
}

bool
Timer::operator> (uint16_t rhs) {
  return _timer > rhs;
}

bool
Timer::timedout (void) {
  return _timer == _timeout;
}

uint16_t
Timer::timeout (void) {
  return _timeout;
}

void
Timer::timeout (uint16_t new_timeout) {
  _timeout = new_timeout;
}
