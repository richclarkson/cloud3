#ifdef UNIT_TEST

#include <unity.h>
#include "TapPressButton.h"

void testTapPressButtonExists() {
    TapPressButton btn;
}

void testTapPressButtonInputIsTrue() {
    TapPressButton btn;
    btn.updateInput(true, 0);
    TEST_ASSERT_TRUE(btn.getCurrentState());
}

void testTapPressButtonInputHasChanged() {
    TapPressButton btn;
    btn.updateInput(true, 0);
    btn.updateInput(false, 100);
    TEST_ASSERT_TRUE(btn.hasStateChanged());
}

void testHasPressTimeElapsed() {
    TapPressButton btn;
    btn.updateInput(true, 0);
    btn.updateInput(true, 100);
    TEST_ASSERT_TRUE(btn.getPressTime() > 0);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(testTapPressButtonExists);
    RUN_TEST(testTapPressButtonInputIsTrue);
    RUN_TEST(testTapPressButtonInputHasChanged);

    UNITY_END();
}

#endif
