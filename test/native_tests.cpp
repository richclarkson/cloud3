#ifdef UNIT_TEST

#include <unity.h>

class TapPressButton {
public:
    void updateInput(bool input) {
        prevButtonState = currentButtonState;
        currentButtonState = input;
    }
    bool getCurrentState() {
        return currentButtonState;
    }
    bool hasChanged() {
        return currentButtonState != prevButtonState;
    }
private:
    bool currentButtonState, prevButtonState;
};

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
    TEST_ASSERT_TRUE(btn.hasChanged());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(testTapPressButtonExists);
    RUN_TEST(testTapPressButtonInputIsTrue);
    RUN_TEST(testTapPressButtonInputHasChanged);

    UNITY_END();
}

#endif
