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
#include "assert.h"

class LeaderTestKey : public LeaderKey {
public:
  uint8_t cnt_FullMatch;

  LeaderTestKey (uint8_t keycode) : LeaderKey (keycode) {
    cnt_FullMatch = 0;
  };

  virtual LookupResult lookup (void) {
    LookupResult r = NOT_FOUND;

    if (seq_length == 0)
      return r;

    if (seq_length == 1)
      return PARTIAL;

    if (sequence[1] == 1 && seq_length >= 2)
      r = PARTIAL;

    if (sequence[1] == 1 && sequence[2] == 2 && seq_length == 3)
      r = MATCH;

    if (sequence[1] == 2 && seq_length == 2)
      r = MATCH;

    return r;
  }

  virtual void action (void) {
    cnt_FullMatch++;
  }

  uint8_t get_seq_len (void) {
    return this->seq_length;
  }

  Timer get_timer (void) {
    return this->timer;
  }

  const uint8_t* get_seq (void) {
    return this->sequence;
  }
};

void
test_leader_timeout (void) {
  LeaderTestKey t = LeaderTestKey (42);

  assert (t.press (42) == false);
  // the first item in the sequence is the leader itself
  assert (t.get_seq_len () == 1);
  assert (t.release (42) == false);
  t.cycle ();

  for (uint8_t i = 0; i <= LEADER_TIMEOUT_DEFAULT; i++) {
    t.cycle ();
  }

  assert (t.get_seq_len () == 0);
}

void
test_leader_not_pressed (void) {
  LeaderTestKey t = LeaderTestKey (42);

  assert (t.press (1) == true);
  assert (t.release (1) == true);
  t.cycle ();
  assert (t.get_timer () == 0);

  for (int i = 0; i < TIMEOUT_DEFAULT * 2; i++)
    t.cycle ();

  assert (t.get_timer () == 0);
}

void
test_leader_interrupt (void) {
  LeaderTestKey t = LeaderTestKey (42);

  t.press (42);
  t.cycle ();
  t.release (42);
  t.cycle ();

  assert (t.press (3) == false);
  t.cycle ();
  assert (t.release (3) == false);
  t.cycle ();

  assert (t.get_timer () == 0);
  assert (t.get_seq_len () == 0);
}

void test_leader_easy_match (void) {
  LeaderTestKey t = LeaderTestKey (42);

  t.press (42);
  t.cycle ();
  t.release (42);
  t.cycle ();

  assert (t.press (2) == false);
  t.cycle ();
  assert (t.release (2) == false);
  t.cycle ();

  assert (t.cnt_FullMatch == 1);
  assert (t.get_timer () == 0);
  assert (t.get_seq_len () == 0);
}

void test_leader_two_seq_match (void) {
  LeaderTestKey t = LeaderTestKey (42);

  t.press (42);
  t.cycle ();
  t.release (42);
  t.cycle ();

  assert (t.press (1) == false);
  t.cycle ();
  assert (t.release (1) == false);
  t.cycle ();

  assert (t.cnt_FullMatch == 0);
  assert (t.get_timer () > 0);
  assert (t.get_seq_len () == 2);

  assert (t.press (2) == false);
  t.cycle ();
  assert (t.release (2) == false);
  t.cycle ();

  assert (t.cnt_FullMatch == 1);
  assert (t.get_timer () == 0);
  assert (t.get_seq_len () == 0);
}

void
test_leader (void) {
  test_leader_not_pressed ();
  test_leader_timeout ();
  test_leader_interrupt ();
  test_leader_easy_match ();
  test_leader_two_seq_match ();
}
