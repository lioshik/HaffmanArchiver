#include <gtest/gtest.h>

#include "../src/BinaryHeap.h"

TEST(TEST_BINARY_HEAP, test1) {
    BinaryHeap<int> heap1;
    heap1.Push(10);
    ASSERT_TRUE(heap1.GetFront() == 10);
    heap1.Push(4);
    ASSERT_TRUE(heap1.GetFront() == 4);
    heap1.Push(4);
    ASSERT_TRUE(heap1.GetFront() == 4);
    heap1.Push(7);
    ASSERT_TRUE(heap1.GetFront() == 4);
    heap1.Push(12);
    ASSERT_TRUE(heap1.GetFront() == 4);
    heap1.Push(2);
    ASSERT_TRUE(heap1.GetFront() == 2);
    heap1.Push(134);
    ASSERT_TRUE(heap1.GetFront() == 2);
    heap1.Push(1);
    ASSERT_TRUE(heap1.GetFront() == 1);
    heap1.Push(15);
    ASSERT_TRUE(heap1.GetFront() == 1);
    heap1.Pop();
    ASSERT_TRUE(heap1.GetFront() == 2);
    heap1.Pop();
    ASSERT_TRUE(heap1.GetFront() == 4);
    heap1.Pop();
    ASSERT_TRUE(heap1.GetFront() == 4);
    heap1.Pop();
    ASSERT_TRUE(heap1.GetFront() == 7);
    heap1.Push(1);
    ASSERT_TRUE(heap1.GetFront() == 1);
    heap1.Pop();
    ASSERT_TRUE(heap1.GetFront() == 7);

    BinaryHeap<int> heap2;
    ASSERT_THROW(heap2.GetFront(), std::out_of_range);
    heap2.Push(1);
    ASSERT_TRUE(heap2.GetFront() == 1);
    heap2.Pop();
    ASSERT_THROW(heap2.GetFront(), std::out_of_range);
}
