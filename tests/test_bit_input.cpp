#include  "gtest/gtest.h"

#include "../src/BitsReader.h"

TEST(TEST_BIT_INPUT, test1) {
    std::string some_str = "abac";
    auto input_stream = std::stringstream(some_str);
    BitsReader reader(input_stream, false);

    const std::vector<bool> expected_bits_a = {0, 1, 1, 0, 0, 0, 0, 1};
    ASSERT_TRUE(reader.ReadBits(8) == expected_bits_a);

    const std::vector<bool> expected_bits_b = {0, 1, 1, 0, 0, 0, 1, 0};
    ASSERT_TRUE(reader.ReadBits(8) == expected_bits_b);

    const std::vector<bool> expected_bits_partial_ac = {0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1};
    ASSERT_TRUE(reader.ReadBits(11) == expected_bits_partial_ac);

    reader.ReadBits(5);
    ASSERT_TRUE(!reader.IsEof());
    reader.NextBit();
    ASSERT_TRUE(reader.IsEof());
}