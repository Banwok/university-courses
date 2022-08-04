#include <telegram/client.h>

std::vector<Message> Client::GetMessages(int offset, int timeout) {
    std::vector<Message> messages;
    int offs = 0;
    if (offset) {
        params["offset"] = std::to_string(offset);
        offs = offset;
    }
    if (timeout) {
        params["timeout"] = std::to_string(timeout);
    }
    offs = offset - offs;
    auto result = URLQuery("getUpdates", "GET", params);
    for (const auto& el : result) {
        auto res = el.get("message", Json::Value());
        if (res.type() == Json::ValueType::nullValue) {
            continue;
        } else {
            Message message{res.get("message_id", Json::Value()).asInt(), res.get("chat", Json::Value()).get("id", Json::Value()).asInt(), 
            el.get("update_id", Json::Value()).asInt(), res.get("text", Json::Value()).asString()};
            messages.push_back(message);
        }
    }
    return messages;
}

void Client::SendMessage(int64_t chat_id, const std::string& text, int64_t reply_to_message_id) {
    if (reply_to_message_id > 0) {
        params["reply_to_message_id"] = std::to_string(reply_to_message_id);
    }
    params["chat_id"] = std::to_string(chat_id);
    params["text"] = text;
    URLQuery("sendMessage", "POST", params);
}

Json::Value Client::URLQuery(const std::string& method, std::string request,
                             const std::unordered_map<std::string, std::string>& params) {
    using http_req = Poco::Net::HTTPRequest;
    using http_res = Poco::Net::HTTPResponse;
    Poco::URI uri(path_ + method);

    if (request == "POST") {
        http_req l_request(http_req::HTTP_POST, uri.getPathAndQuery());
        std::stringstream ss;
        Poco::JSON::Object message_info;
        for (const auto& el : params) {
            message_info.set(el.first, el.second);
        }

        l_request.setContentType("application/json");
        
        message_info.stringify(ss);
        l_request.setContentLength(ss.str().size());
        message_info.stringify(session_->sendRequest(l_request));
    }

    if (request == "GET") {
        for (const auto& el : params) {
            uri.addQueryParameter(el.first, el.second);
        }
        http_req l_request(http_req::HTTP_GET, uri.getPathAndQuery());
        session_->sendRequest(l_request);
    }

    http_res response;
    std::istream& rs = session_->receiveResponse(response);
    if (response.getStatus() != http_res::HTTPStatus::HTTP_OK) {
        throw "request error";
    }
    Json::CharReaderBuilder builder;
    Json::Value value;
    JSONCPP_STRING errs;
    parseFromStream(builder, rs, &value, &errs);
    return value["result"];
}
