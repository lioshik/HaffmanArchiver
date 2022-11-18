#include "gtest/gtest.h"

#include <vector>
#include <unordered_map>

#include "../src/HaffmanAlgorithm.h"

TEST(TEST_HAFFMAN_ALGORITHM, test1) {
    std::unordered_map<std::vector<bool>, size_t> symbols_frequency;
    const std::vector<bool> bits_a = {0, 1, 1, 0, 0, 0, 0, 1};
    const std::vector<bool> bits_b = {0, 1, 1, 0, 0, 0, 1, 0};
    const std::vector<bool> bits_c = {0, 1, 1, 0, 0, 0, 1, 1};
    const std::vector<bool> bits_d = {0, 1, 1, 0, 0, 1, 0, 0};
    const std::vector<bool> bits_e = {0, 1, 1, 0, 0, 1, 0, 1};

    symbols_frequency[bits_a] = 15;
    symbols_frequency[bits_b] = 7;
    symbols_frequency[bits_c] = 6;
    symbols_frequency[bits_d] = 6;
    symbols_frequency[bits_e] = 5;

    HaffmanAlgorithm algorithm(symbols_frequency);
    std::unordered_map<std::vector<bool>, std::vector<bool>> expected_result;
    expected_result[bits_a] = {0};
    expected_result[bits_b] = {1, 0, 0};
    expected_result[bits_c] = {1, 0, 1};
    expected_result[bits_d] = {1, 1, 0};
    expected_result[bits_e] = {1, 1, 1};

    auto result_canonical_dictionary = algorithm.GetCanonicalDict();
    ASSERT_TRUE(result_canonical_dictionary == expected_result);

    std::unordered_map<std::vector<bool>, std::vector<bool>> wrong_expected_result;
    wrong_expected_result[bits_a] = {0};
    wrong_expected_result[bits_b] = {1, 0, 0};
    wrong_expected_result[bits_c] = {1, 1, 0};
    wrong_expected_result[bits_d] = {1, 0, 1};
    wrong_expected_result[bits_e] = {1, 1, 1};
    ASSERT_TRUE(HaffmanAlgorithm(symbols_frequency).GetCanonicalDict() != wrong_expected_result);
    ASSERT_TRUE(HaffmanAlgorithm(symbols_frequency).GetCanonicalDict() == expected_result);
}
