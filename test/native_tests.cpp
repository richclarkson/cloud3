#ifdef UNIT_TEST

#include <unity.h>

class TapPressButton {
public:
    void updateInput(bool _input) {
        input = _input;
    }
};

void testTapPressButtonExists() {
    TapPressButton btn;
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(testTapPressButtonExists);

    UNITY_END();
}

#endif
