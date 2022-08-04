#pragma once
#include <fstream>
#include <memory>
#include <unordered_map>
#include <string>
#include <Poco/URI.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/JSON/Parser.h>
#include <jsoncpp/json/json.h>

struct Message {
    int id_;
    int ch_id_;
    int up_id_;
    std::string str_;
};

class Client {
public:
    Client(const std::string& url, const std::string& token) {
        path_ = "/bot" + token + "/";
        Poco::URI uri(url);
        if (uri.getScheme() == "http") {
            session_ = std::make_shared<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
        } else if (uri.getScheme() == "https") {
            session_ = std::shared_ptr<Poco::Net::HTTPClientSession>(
                new Poco::Net::HTTPSClientSession(uri.getHost(), uri.getPort()));
        } else {
            throw "URI error";
        }
    }
    std::vector<Message> GetMessages(int offset = 0, int timeout = 0);
    void SendMessage(int64_t chat_id, const std::string& text, int64_t reply_to_message_id = 0);
    Json::Value URLQuery(const std::string& method, std::string request = "GET",
                         const std::unordered_map<std::string, std::string>& params = {});
    std::unordered_map<std::string, std::string> params;

private:
    std::shared_ptr<Poco::Net::HTTPClientSession> session_;
    std::string path_;
    int timeout_ = 20;
};