#include "TapDance.h"
#include "assert.h"

#include <iostream>

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
  bool is_finished (void) { return this->finished; };
  bool is_interrupted (void) { return this->interrupted; };

  void onEachTap () { cnt_onEach++; };
  void onFinish () {
    cnt_onFinish++;
    if (this->interrupted)
      cnt_Interrupted++;
  };
  void onReset () { cnt_onReset++; };
};

// ---- Tapping only on the tap-dance key ----
void
test_tapping_on_a_single_key (void) {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Initially, the count in zero, and states are all off
  assert (t.get_count () == 0);
  assert (t.is_pressed () == false);
  assert (t.is_finished () == false);
  assert (t.is_interrupted () == false);
  assert (t.cnt_onEach == 0);
  assert (t.cnt_onFinish == 0);
  assert (t.cnt_onReset == 0);

  // Tapping the key once should increment the counter by one
  t.press (42);
  assert (t.get_count () == 1);
  // The key is also pressed now
  assert (t.is_pressed ());
  // The onEach callback gets called, once
  assert (t.cnt_onEach == 1);
  // Releasing the key should not increment the counter
  t.release (42);
  assert (t.get_count () == 1);
  // ...but it should not be pressed anymore
  assert (t.is_pressed() == false);

  // Tapping it again, should increment the counter again
  t.press (42);
  assert (t.get_count () == 2);
  // and the key shall be pressed
  assert (t.is_pressed ());
  // and onEachTap gets called a second time
  assert (t.cnt_onEach == 2);
  t.release (42);

  // Cycling it 39 times (~200ms) shall almost time it out...
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    assert (t.is_finished () == false);
  }
  assert (t.cnt_onFinish == 0);
  assert (t.cnt_onReset == 0);
  // onEachTap is still on two
  assert (t.cnt_onEach == 2);

  // Cycling a 40th time, times it out
  t.cycle ();
  // onFinish is called once
  assert (t.cnt_onFinish == 1);
  // onReset too
  assert (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  assert (t.get_count () == 0);
  assert (t.is_pressed () == false);
  assert (t.is_finished () == false);
  assert (t.is_interrupted () == false);
}

void
test_one_tap_with_interrupt_after () {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Tapping the key once should increment the counter by one
  t.press (42);
  assert (t.get_count () == 1);
  // The key is also pressed now
  assert (t.is_pressed ());
  // The onEach callback gets called, once
  assert (t.cnt_onEach == 1);
  // Releasing the key should not increment the counter
  t.release (42);
  assert (t.get_count () == 1);
  // ...but it should not be pressed anymore
  assert (t.is_pressed() == false);

  // Tapping another key shall interrupt the dance
  t.press (13);

  // onEach should still be one, because we did not tap the dance-key
  assert (t.cnt_onEach == 1);
  // finish and reset should have been called, because we're done with the key for now.
  assert (t.cnt_onFinish == 1);
  assert (t.cnt_onReset == 1);
  assert (t.cnt_Interrupted == 1);

  // at this point, everything should be reset to default, save our test counters
  assert (t.get_count () == 0);
  assert (t.is_pressed () == false);
  assert (t.is_finished () == false);
  assert (t.is_interrupted () == false);
}

void
test_one_tap_with_interrupt_during_release () {
  TapDanceTestKey t = TapDanceTestKey(42);

  // Tapping the key once should increment the counter by one
  t.press (42);
  assert (t.get_count () == 1);
  // The key is also pressed now
  assert (t.is_pressed ());
  // The onEach callback gets called, once
  assert (t.cnt_onEach == 1);

  // Tapping another key shall interrupt the dance
  t.press (13);

  // onEach should still be one, because we did not tap the dance-key
  assert (t.cnt_onEach == 1);
  // finish has to be called once, because we finished with the key; interrupt also set
  assert (t.cnt_onFinish == 1);
  assert (t.cnt_Interrupted == 1);
  // reset should also be called, because of the interrupt
  assert (t.cnt_onReset == 1);

  // at this point, everything should be reset to default, save our test counters
  assert (t.get_count () == 0);
  assert (t.is_pressed () == false);
  assert (t.is_finished () == false);
  assert (t.is_interrupted () == false);
}

int
main (void) {

  test_tapping_on_a_single_key ();
  test_one_tap_with_interrupt_after ();
  test_one_tap_with_interrupt_during_release ();

  return 0;
}
