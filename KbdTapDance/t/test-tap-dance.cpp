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

#include "TapDance.h"

class TapDanceTestKey : public TapDanceKey {
public:
  TapDanceTestKey (uint8_t keycode) : TapDanceKey (keycode) {
    cnt_onEach = 0;
    cnt_onFinish = 0;
    cnt_onReset = 0;
    cnt_Interrupted = 0;
  };

  uint8_t cnt_onEach, cnt_onFinish, cnt_onReset, cnt_Interrupted;

  uint8_t get_count (void) { return this->count; };
  bool is_pressed (void) { return this->pressed; };
  bool is_interrupted (void) { return this->interrupted; };
  bool is_timedout (void) { return this->timedout; };

  void onEachTap () { cnt_onEach++; };
  void onFinish () {
    cnt_onFinish++;
    if (this->interrupted)
      cnt_Interrupted++;
  };
  void onReset () { cnt_onReset++; };
};

class TapDanceTestDoubleKey : public TapDanceDoubleKey {
protected:
  virtual void register_code (uint8_t kc) { last_reg_kc = kc; };
  virtual void unregister_code (uint8_t kc) { last_unreg_kc = kc; };

public:
  TapDanceTestDoubleKey (uint8_t code, uint8_t kc1, uint8_t kc2) : TapDanceDoubleKey (code, kc1, kc2) {
    cnt_onFinish = 0;
    cnt_onReset = 0;
  }

  uint8_t cnt_onFinish, cnt_onReset, last_reg_kc, last_unreg_kc;

  uint8_t get_count (void) { return this->count; };
  uint8_t get_last_reg (void) { return this->last_reg_kc; };
  uint8_t get_last_unreg (void) { return this->last_unreg_kc; };

  void onFinish () { cnt_onFinish++; TapDanceDoubleKey::onFinish (); };
  void onReset () { cnt_onReset++; TapDanceDoubleKey::onReset (); };
};

TEST_GROUP (TapDance) {
};

// ---- Tapping only on the tap-dance key ----
TEST (TapDance, tapping_on_a_single_key) {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Initially, the count in zero, and states are all off
  CHECK (t.get_count () == 0);
  CHECK (t.is_pressed () == false);
  CHECK (t.is_timedout() == false);
  CHECK (t.is_interrupted () == false);
  CHECK (t.cnt_onEach == 0);
  CHECK (t.cnt_onFinish == 0);
  CHECK (t.cnt_onReset == 0);

  // Tapping the key once should increment the counter by one
  t.press (42);
  CHECK (t.get_count () == 1);
  // The key is also pressed now
  CHECK (t.is_pressed ());
  // The onEach callback gets called, once
  CHECK (t.cnt_onEach == 1);
  // Releasing the key should not increment the counter
  t.release (42);
  CHECK (t.get_count () == 1);
  // ...but it should not be pressed anymore
  CHECK (t.is_pressed() == false);

  // Tapping it again, should increment the counter again
  t.press (42);
  CHECK (t.get_count () == 2);
  // and the key shall be pressed
  CHECK (t.is_pressed ());
  // and onEachTap gets called a second time
  CHECK (t.cnt_onEach == 2);
  t.release (42);

  // Cycling it 39 times (~200ms) shall almost time it out...
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    CHECK (t.is_timedout() == false);
  }
  CHECK (t.cnt_onFinish == 0);
  CHECK (t.cnt_onReset == 0);
  // onEachTap is still on two
  CHECK (t.cnt_onEach == 2);

  // Cycling a 40th time, times it out
  t.cycle ();
  // onFinish is called once
  CHECK (t.cnt_onFinish == 1);
  // onReset too
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);
  CHECK (t.is_pressed () == false);
  CHECK (t.is_timedout () == false);
  CHECK (t.is_interrupted () == false);
}

TEST (TapDance, tapping_on_a_single_key_with_timeout_during_press) {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Tapping the key once should increment the counter by one
  t.press (42);
  CHECK (t.get_count () == 1);
  // The key is also pressed now
  CHECK (t.is_pressed ());
  // The onEach callback gets called, once
  CHECK (t.cnt_onEach == 1);

  // Cycling it 39 times (~200ms) shall almost time it out...
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    CHECK (t.is_timedout () == false);
  }
  CHECK (t.cnt_onFinish == 0);
  CHECK (t.cnt_onReset == 0);
  // onEachTap is still on one
  CHECK (t.cnt_onEach == 1);

  // Cycling a 40th time, times it out
  t.cycle ();
  // onFinish is called once
  CHECK (t.cnt_onFinish == 1);
  // onReset not called yet, because the key is still pressed
  CHECK (t.cnt_onReset == 0);

  // at this point, the key is still active
  CHECK (t.get_count () == 1);
  CHECK (t.is_pressed ());
  CHECK (t.is_timedout ());
  CHECK (t.is_interrupted () == false);

  // timing it out even further still does not reset it
  for (int c = 0; c < 80; c++) {
    t.cycle ();
    CHECK (t.is_timedout () == true);
  }
  // onFinish is still only called once
  CHECK (t.cnt_onFinish == 1);
  // ...and reset not called yet
  CHECK (t.cnt_onReset == 0);

  // releasing the key resets us
  t.release (42);
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);
  CHECK (t.is_pressed () == false);
  CHECK (t.is_timedout () == false);
  CHECK (t.is_interrupted () == false);
}

TEST (TapDance, tap_with_interrupt_after) {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Tapping the key once should increment the counter by one
  t.press (42);
  CHECK (t.get_count () == 1);
  // The key is also pressed now
  CHECK (t.is_pressed ());
  // The onEach callback gets called, once
  CHECK (t.cnt_onEach == 1);
  // Releasing the key should not increment the counter
  t.release (42);
  CHECK (t.get_count () == 1);
  // ...but it should not be pressed anymore
  CHECK (t.is_pressed() == false);

  // Tapping another key shall interrupt the dance
  t.press (13);

  // onEach should still be one, because we did not tap the dance-key
  CHECK (t.cnt_onEach == 1);
  // finish and reset should have been called, because we're done with the key for now.
  CHECK (t.cnt_onFinish == 1);
  CHECK (t.cnt_onReset == 1);
  CHECK (t.cnt_Interrupted == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);
  CHECK (t.is_pressed () == false);
  CHECK (t.is_timedout () == false);
  CHECK (t.is_interrupted () == false);
}

TEST (TapDance, one_tap_with_interrupt_during_release) {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Tapping the key once should increment the counter by one
  t.press (42);
  CHECK (t.get_count () == 1);
  // The key is also pressed now
  CHECK (t.is_pressed ());
  // The onEach callback gets called, once
  CHECK (t.cnt_onEach == 1);

  // Tapping another key shall interrupt the dance
  t.press (13);

  // onEach should still be one, because we did not tap the dance-key
  CHECK (t.cnt_onEach == 1);
  // finish has to be called once, because we finished with the key; interrupt also set
  CHECK (t.cnt_onFinish == 1);
  CHECK (t.cnt_Interrupted == 1);
  // reset should also be called, because of the interrupt
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);
  CHECK (t.is_pressed () == false);
  CHECK (t.is_timedout () == false);
  CHECK (t.is_interrupted () == false);
}

TEST (TapDance, hold_with_interrupt) {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Tapping the key once should increment the counter by one
  t.press (42);
  CHECK (t.get_count () == 1);
  // The key is also pressed now
  CHECK (t.is_pressed ());
  // The onEach callback gets called, once
  CHECK (t.cnt_onEach == 1);

  // Cycling it 39 times (~200ms) shall almost time it out...
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    CHECK (t.is_timedout () == false);
  }
  CHECK (t.cnt_onFinish == 0);
  CHECK (t.cnt_onReset == 0);
  // onEachTap is still on one
  CHECK (t.cnt_onEach == 1);

  // Cycling a 40th time, times it out
  t.cycle ();
  // onFinish is called once
  CHECK (t.cnt_onFinish == 1);
  // onReset not called yet, because the key is still pressed
  CHECK (t.cnt_onReset == 0);

  // at this point, the key is still active
  CHECK (t.get_count () == 1);
  CHECK (t.is_pressed ());
  CHECK (t.is_timedout ());
  CHECK (t.is_interrupted () == false);

  // pressing any other key interrupts us
  t.press (13);

  // ...but since we are held, no reset is called
  CHECK (t.cnt_onReset == 0);
  // the interrupted flag is set, however
  CHECK (t.is_interrupted ());

  // releasing the key resets us
  t.release (42);
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);
  CHECK (t.is_pressed () == false);
  CHECK (t.is_timedout () == false);
  CHECK (t.is_interrupted () == false);
}

TEST_GROUP (TapDanceDouble) {
};

TEST (TapDanceDouble, one_tap) {
  TapDanceTestDoubleKey t = TapDanceTestDoubleKey (42, 2, 27);

  // tap-and-release, followed by a timeout
  t.press (42);
  t.release (42);
  CHECK (t.get_count () == 1);

  for (int c = 0; c < 40; c++) {
    t.cycle ();
  }

  // finished and reset
  CHECK (t.cnt_onFinish == 1);
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);

  // the registered keycode shall be the first one
  CHECK (t.get_last_reg () == 2);
  CHECK (t.get_last_unreg () == 2);
}

TEST (TapDanceDouble, double_tap) {
  TapDanceTestDoubleKey t = TapDanceTestDoubleKey (42, 2, 27);

  // tap-and-release, followed by a timeout
  t.press (42);
  t.release (42);
  t.press (42);
  t.release (42);
  CHECK (t.get_count () == 2);

  for (int c = 0; c < 40; c++) {
    t.cycle ();
  }

  // finished and reset
  CHECK (t.cnt_onFinish == 1);
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);

  // the registered keycode shall be the first one
  CHECK (t.get_last_reg () == 27);
  CHECK (t.get_last_unreg () == 27);
}

TEST (TapDance, tap_with_concurrent_release) {
  TapDanceTestKey t = TapDanceTestKey (42);

  // we first press another key
  t.press (2);

  // we then press the tap-dance key
  t.press (42);

  // count shall be one, and onEachTap should have been called
  CHECK (t.get_count () == 1);
  CHECK (t.cnt_onEach == 1);

  // we then release another key
  t.release (2);

  // count, onEach and everything else should be the same, keyrelease does not
  // interrupt us
  CHECK (t.cnt_onFinish == 0);
  CHECK (t.cnt_onReset == 0);
  CHECK (t.get_count () == 1);
  CHECK (t.cnt_onEach == 1);

  // releasing the tap-dance key finishes off the sequence, after a timeout
  t.release (42);
  for (int c = 0; c < 40; c++) {
    t.cycle ();
  }

  // finished and reset
  CHECK (t.cnt_onFinish == 1);
  CHECK (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  CHECK (t.get_count () == 0);
}

TEST (TapDance, callback_signal) {
  TapDanceTestKey t = TapDanceTestKey (42);

  CHECK (t.press (1) == true);
  CHECK (t.release (1) == true);

  CHECK (t.press (42) == false);
  CHECK (t.release (42) == false);
  t.cycle ();
  CHECK (t.press (1) == true);
  CHECK (t.release (1) == true);
}
