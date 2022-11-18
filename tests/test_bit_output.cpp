#include "gtest/gtest.h"

#include "../src/BitsWriter.h"

TEST(TEST_BIT_OUTPUT, test1) {
    std::stringstream some_string_stream;
    BitsWriter writer(some_string_stream, false);
    const std::vector<bool> expected_bits_a = {0, 1, 1, 0, 0, 0, 0, 1};
    const std::vector<bool> expected_bits_b = {0, 1, 1, 0, 0, 0, 1, 0};
    const std::vector<bool> expected_bits_ab = {0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0};

    const std::vector<bool> expected_bits_partial_ab_1 = {0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0};
    const std::vector<bool> expected_bits_partial_ab_2 = {0, 0, 1, 0};

    writer.WriteBits(expected_bits_a);
    ASSERT_TRUE(some_string_stream.str() == "a");
    writer.WriteBits(expected_bits_b);
    ASSERT_TRUE(some_string_stream.str() == "ab");
    writer.WriteBits(expected_bits_ab);
    ASSERT_TRUE(some_string_stream.str() == "abab");

    writer.WriteBits(expected_bits_partial_ab_1);
    writer.WriteBits(expected_bits_partial_ab_2);
    ASSERT_TRUE(some_string_stream.str() == "ababab");
}