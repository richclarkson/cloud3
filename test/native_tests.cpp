#ifdef UNIT_TEST

#include <unity.h>
#include "TapPressButton.h"

void testTapPressButtonExists() {
    TapPressButton btn;
}

void testTapPressButtonInputIsTrue() {
    TapPressButton btn;
    btn.updateInput(true);
    TEST_ASSERT_TRUE(btn.getCurrentState());
}

void testTapPressButtonInputHasChanged() {
    TapPressButton btn;
    btn.updateInput(true);
    btn.updateInput(false);
    TEST_ASSERT_TRUE(btn.hasStateChanged());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(testTapPressButtonExists);
    RUN_TEST(testTapPressButtonInputIsTrue);
    RUN_TEST(testTapPressButtonInputHasChanged);

    UNITY_END();
}

#endif
