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

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "Timer.h"

TEST_GROUP (TimeOut) {
};

TEST (TimeOut, timing_out) {
  Timer t = Timer ();

  CHECK (t == 0);
  t++;
  CHECK (t == 1);

  for (int i = 0; i < TIMEOUT_DEFAULT; i++) {
    t++;
  }

  CHECK (t == TIMEOUT_DEFAULT);
  CHECK (t.timedout () == true);
}

TEST (TimeOut, custom_timeout) {
  Timer t = Timer (TIMEOUT_DEFAULT * 2);

  CHECK (t == 0);

  for (int i = 0; i < TIMEOUT_DEFAULT + 2; i++) {
    t++;
  }
  CHECK (t.timedout () == false);

  for (int i = 0; i < TIMEOUT_DEFAULT; i++) {
    t++;
  }
  CHECK (t.timedout () == true);
}

TEST (TimeOut, reset) {
  Timer t = Timer ();

  for (int i = 0; i < TIMEOUT_DEFAULT * 2; i++)
    t++;

  t = 0;
  CHECK (t == 0);
  CHECK (t.timedout () == false);
}

TEST (TimeOut, in_flight_timeout_change) {
  Timer t = Timer ();

  for (int i = 0; i < TIMEOUT_DEFAULT - 1; i++)
    t++;

  t (TIMEOUT_DEFAULT * 2);

  for (int i = 0; i < TIMEOUT_DEFAULT; i++)
    t++;

  CHECK (t.timedout () == false);

  t++;
  t++;
  t++;

  CHECK (t.timedout () == true);
}
