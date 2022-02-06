#include "../../lib/Unity/src/unity.h"
#include "test_core.hpp"

void setUp() {
    // Required for the unity framework
}

void tearDown() {
    // Required for the unity framework
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_ParseColorFromString_comma_separated_simple);
    RUN_TEST(test_ParseColorFromString_comma_separated_limit);
    RUN_TEST(test_ParseColorFromString_comma_separated_overflow);
    RUN_TEST(test_ParseColorFromString_hashtag_simple);
    RUN_TEST(test_ParseColorFromString_hashtag_limit);
    RUN_TEST(test_ParseColorFromString_hashtag_overflow);
    RUN_TEST(test_RandomColor_without_seed);
    return UNITY_END();
}
