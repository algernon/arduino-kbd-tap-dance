#include "OneShot.h"
#include "assert.h"

class OneShotTestKey : public OneShotKey {
protected:
  virtual void register_code (uint8_t kc) { last_reg_kc = kc; };
  virtual void unregister_code (uint8_t kc) { last_unreg_kc = kc; };

public:
  OneShotTestKey (uint8_t keycode) : OneShotKey (keycode) {
    cnt_onActivate = 0;
    cnt_onDeactivate = 0;
    last_reg_kc = 0;
    last_unreg_kc = 0;
  };

  uint8_t cnt_onActivate, cnt_onDeactivate, last_reg_kc, last_unreg_kc;

  void onActivate () { cnt_onActivate++; };
  void onDeactivate () { cnt_onDeactivate++; };
};

void
test_oneshot_activate_deactivate_with_timeout (void) {
  OneShotTestKey t = OneShotTestKey(42);

  t.press (1);
  t.release (1);
  assert (t.cnt_onActivate == 0);

  t.press (42);
  // the key should activate immediately
  assert (t.cnt_onActivate == 1);
  assert (t.last_reg_kc == 42);

  // releasing the key shall not immediately unregister
  t.release (42);
  assert (t.last_unreg_kc == 0);

  // timing it out shall deactivate the key
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    assert (t.cnt_onDeactivate == 0);
  }
  t.cycle ();
  assert (t.cnt_onDeactivate == 1);
  assert (t.last_unreg_kc == 42);
}

int
main (void) {

  test_oneshot_activate_deactivate_with_timeout ();

  return 0;
}
