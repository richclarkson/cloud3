#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>

StateManager sm;

void setUp(void) {
    sm = StateManager();
}

void tearDown(void) {

}

int main() {
    UNITY_BEGIN();

    UNITY_END();
}

#endif
