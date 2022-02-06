#include "../../lib/Core/src/Core.h"
#include "../../lib/Unity/src/unity.h"

void test_ParseColorFromString_comma_separated_simple() {
    int colors[3] = {0, 0, 0};
    char input[6] = "1,2,3";
    Core::ParseColorFromString(input, colors);
    TEST_ASSERT_EQUAL(1, colors[0]);
    TEST_ASSERT_EQUAL(2, colors[1]);
    TEST_ASSERT_EQUAL(3, colors[2]);
}

void test_ParseColorFromString_comma_separated_limit() {
    int colors[3] = {0, 0, 0};
    char input[12] = "255,255,255";
    Core::ParseColorFromString(input, colors);
    TEST_ASSERT_EQUAL(255, colors[0]);
    TEST_ASSERT_EQUAL(255, colors[1]);
    TEST_ASSERT_EQUAL(255, colors[2]);
    char inputBis[6] = "0,0,0";
    Core::ParseColorFromString(inputBis, colors);
    TEST_ASSERT_EQUAL(0, colors[0]);
    TEST_ASSERT_EQUAL(0, colors[1]);
    TEST_ASSERT_EQUAL(0, colors[2]);
}

void test_ParseColorFromString_comma_separated_overflow() {
    int colors[3] = {0, 0, 0};
    char input[12] = "400,600,900";
    Core::ParseColorFromString(input, colors);
    TEST_ASSERT_LESS_THAN(256, colors[0]);
    TEST_ASSERT_LESS_THAN(256, colors[1]);
    TEST_ASSERT_LESS_THAN(256, colors[2]);
}

void test_ParseColorFromString_hashtag_simple() {
    int colors[3] = {0, 0, 0};
    char input[8] = "#aabbcc";
    Core::ParseColorFromString(input, colors);
    TEST_ASSERT_EQUAL(170, colors[0]);
    TEST_ASSERT_EQUAL(187, colors[1]);
    TEST_ASSERT_EQUAL(204, colors[2]);
}

void test_ParseColorFromString_hashtag_limit() {
    int colors[3] = {0, 0, 0};
    char input[12] = "#ffffff";
    Core::ParseColorFromString(input, colors);
    TEST_ASSERT_EQUAL(255, colors[0]);
    TEST_ASSERT_EQUAL(255, colors[1]);
    TEST_ASSERT_EQUAL(255, colors[2]);
    char inputBis[12] = "#000000";
    Core::ParseColorFromString(inputBis, colors);
    TEST_ASSERT_EQUAL(0, colors[0]);
    TEST_ASSERT_EQUAL(0, colors[1]);
    TEST_ASSERT_EQUAL(0, colors[2]);
}

void test_ParseColorFromString_hashtag_overflow() {
    int colors[3] = {0, 0, 0};
    char input[12] = "#hhiijj";
    Core::ParseColorFromString(input, colors);
    TEST_ASSERT_EQUAL(0, colors[0]);
    TEST_ASSERT_EQUAL(0, colors[1]);
    TEST_ASSERT_EQUAL(0, colors[2]);
}

void test_RandomColor_without_seed() {
    auto colors = Core::RandomColor();
    TEST_ASSERT_LESS_THAN(256, colors[0]);
    TEST_ASSERT_LESS_THAN(256, colors[1]);
    TEST_ASSERT_LESS_THAN(256, colors[2]);
}