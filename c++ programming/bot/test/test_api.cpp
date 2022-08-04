#include <telegram/client.h>
#include "telegram/fake.h"
#include "catch.hpp"

TEST_CASE("Single getMe") {
    telegram::FakeServer fake("Single getMe");
    fake.Start();
    Client client(fake.GetUrl(), "123");

    auto res = client.URLQuery("getMe");

    fake.StopAndCheckExpectations();
}

TEST_CASE("getMe error handling") {
    telegram::FakeServer fake("getMe error handling");
    fake.Start();
    Client client(fake.GetUrl(), "123");
    REQUIRE_THROWS(client.URLQuery("getMe"));
    REQUIRE_THROWS(client.URLQuery("getMe"));

    fake.StopAndCheckExpectations();
}

TEST_CASE("Single getUpdates and send messages") {
    telegram::FakeServer fake("Single getUpdates and send messages");
    fake.Start();
    Client client(fake.GetUrl(), "123");

    auto messages = client.GetMessages(0);
    client.SendMessage(messages[0].ch_id_, "Hi!");
    client.SendMessage(messages[1].ch_id_, "Reply", messages[1].id_);
    client.SendMessage(messages[1].ch_id_, "Reply", messages[1].id_);

    fake.StopAndCheckExpectations();
}

TEST_CASE("Handle getUpdates offset") {
    telegram::FakeServer fake("Handle getUpdates offset");
    fake.Start();
    Client client(fake.GetUrl(), "123");
    auto messages = client.GetMessages(0, 5);
    int max_update_id = std::max(messages[0].up_id_, messages[1].up_id_);
    client.GetMessages(max_update_id + 1, 5);
    client.GetMessages(max_update_id + 1, 5);

    fake.StopAndCheckExpectations();
}