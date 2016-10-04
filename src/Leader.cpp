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

#include "Leader.h"

#include <string.h>

LeaderKey::LeaderKey (uint8_t code) {
  keycode = code;
  leader_timeout = LEADER_TIMEOUT_DEFAULT;
  _reset ();
}

LeaderKey::LeaderKey (uint8_t code, uint16_t timeout) : LeaderKey (code) {
  this->timeout (timeout);
}

void
LeaderKey::timeout (uint16_t new_timeout) {
  this->leader_timeout = new_timeout;
}

uint16_t
LeaderKey::timeout (void) {
  return this->leader_timeout;
}

void
LeaderKey::_reset (void) {
  timer = 0;
  seq_length = 0;
  need_reset = false;
  memset (sequence, 0, LEADER_SEQUENCE_LENGTH_MAX);
}

bool
LeaderKey::press (uint8_t code) {
  if (seq_length == 0 && code != keycode)
    return true;

  sequence[seq_length] = code;
  seq_length++;

  switch (lookup ()) {
  case NOT_FOUND:
    need_reset = true;
    break;
  case MATCH:
    need_reset = true;
    action ();
    break;
  case PARTIAL:
    break;
  }

  return false;
}

bool
LeaderKey::release (uint8_t code) {
  if (need_reset) {
    _reset ();
    return false;
  }

  if (code != keycode)
    return seq_length == 0;

  return false;
}

void
LeaderKey::cycle (void) {
  if (seq_length == 0)
    return;

  timer++;

  if (timer >= this->leader_timeout) {
    _reset ();
  }
}
