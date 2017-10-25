#ifdef UNIT_TEST

#include <unity.h>

class TapPressButton {
public:
    bool input;
    void updateInput(bool _input) {
        input = _input;
    }
};

void testTapPressButtonExists() {
    TapPressButton btn;
}

void testTapPressButtonInputIsTrue() {
    TapPressButton btn;
    btn.updateInput(true);
    TEST_ASSERT_TRUE(btn.input);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(testTapPressButtonExists);
    RUN_TEST(testTapPressButtonInputIsTrue);

    UNITY_END();
}

#endif
