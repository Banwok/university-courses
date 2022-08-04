#include <catch.hpp>
#include <util.h>
#include <ring_buffer.h>

TEST_CASE("Empty") {
    RingBuffer buffer(1);

    REQUIRE(0u == buffer.Size());
    REQUIRE(true == buffer.Empty());
}

TEST_CASE("PushAndPop") {
    RingBuffer buffer(2);

    int i;
    REQUIRE(true == buffer.TryPush(0));
    REQUIRE(true == buffer.TryPush(1));
    REQUIRE(false == buffer.TryPush(2));

    REQUIRE(2u == buffer.Size());
    REQUIRE(false == buffer.Empty());

    REQUIRE(true == buffer.TryPop(&i));
    REQUIRE(0 == i);
    REQUIRE(true == buffer.TryPop(&i));
    REQUIRE(1 == i);

    REQUIRE(false == buffer.TryPop(&i));
    REQUIRE(0u == buffer.Size());
    REQUIRE(true == buffer.Empty());
}

TEST_CASE("Random") {
    RandomGenerator rnd(73467534);
    RingBuffer buffer(10);

    int next_element = 0;
    int next_received_element = 0;
    for (int i = 0; i < 100000; ++i) {
        if (rnd.GenInt(0, 1) == 0) {
            if (buffer.TryPush(next_element)) {
                next_element++;
            }
        } else {
            int element;
            if (buffer.TryPop(&element)) {
                REQUIRE(next_received_element == element);
                next_received_element++;
            }
        }
    }
}
