#include "sequence/PriorityQueue.h"
#include <gtest/gtest.h>
#include <functional>
#include <string>

TEST(PriorityQueueTest, EnqueueDequeueMinHeap) {
    PriorityQueue<std::string, int> pq;
    pq.Enqueue("high", 10);
    pq.Enqueue("low", 1);
    pq.Enqueue("medium", 5);

    EXPECT_EQ(pq.Size(), 3);
    EXPECT_EQ(pq.PeekFirst(), "low");
    EXPECT_EQ(pq.Dequeue(), "low");
    EXPECT_EQ(pq.Dequeue(), "medium");
    EXPECT_EQ(pq.Dequeue(), "high");
    EXPECT_TRUE(pq.IsEmpty());
}

TEST(PriorityQueueTest, MaxHeapComparator) {
    auto maxComp = [](int a, int b) { return a > b; };
    PriorityQueue<std::string, int> pq(maxComp);

    pq.Enqueue("A", 100);
    pq.Enqueue("B", 10);
    pq.Enqueue("C", 50);

    EXPECT_EQ(pq.Dequeue(), "A");
    EXPECT_EQ(pq.Dequeue(), "C");
}

TEST(PriorityQueueTest, PeekMethods) {
    PriorityQueue<int, double> pq;

    pq.Enqueue(100, 3.5);
    pq.Enqueue(200, 1.2);
    pq.Enqueue(300, 4.0);

    EXPECT_EQ(pq.PeekFirst(), 200);
    EXPECT_EQ(pq.PeekLast(), 300);
    EXPECT_EQ(pq.Peek(1), 100);
}