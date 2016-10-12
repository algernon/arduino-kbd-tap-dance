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
#include "assert.h"

void
test_timer_timing_out (void) {
  Timer t = Timer ();

  assert (t == 0);
  t++;
  assert (t == 1);

  for (int i = 0; i < TIMEOUT_DEFAULT; i++) {
    t++;
  }

  assert (t == TIMEOUT_DEFAULT);
  assert (t.timedout () == true);
}

void
test_timer_custom_timeout (void) {
  Timer t = Timer (TIMEOUT_DEFAULT * 2);

  assert (t == 0);

  for (int i = 0; i < TIMEOUT_DEFAULT + 2; i++) {
    t++;
  }
  assert (t.timedout () == false);

  for (int i = 0; i < TIMEOUT_DEFAULT; i++) {
    t++;
  }
  assert (t.timedout () == true);
}

void
test_timer_reset (void) {
  Timer t = Timer ();

  for (int i = 0; i < TIMEOUT_DEFAULT * 2; i++)
    t++;

  t.reset ();
  assert (t == 0);
  assert (t.timedout () == false);
}

void
test_timer_in_flight_timeout_change (void) {
  Timer t = Timer ();

  for (int i = 0; i < TIMEOUT_DEFAULT - 1; i++)
    t++;

  t.timeout (TIMEOUT_DEFAULT * 2);

  for (int i = 0; i < TIMEOUT_DEFAULT; i++)
    t++;

  assert (t.timedout () == false);

  t++;
  t++;
  t++;

  assert (t.timedout () == true);
}

int
main (void) {

  test_timer_timing_out ();
  test_timer_custom_timeout ();
  test_timer_reset ();
  test_timer_in_flight_timeout_change ();

  return 0;
}
