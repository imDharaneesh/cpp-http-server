#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <unordered_map>

class HttpResponse {
public:
    int status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    HttpResponse() {
        headers["Content-Type"] = "text/plain";
        headers["Connection"] = "close";
    }

    static HttpResponse json(int code, const std::string& json_body);
    static HttpResponse text(int code, const std::string& text_body);
    static HttpResponse status(int code, const std::string& msg = "");

    HttpResponse& set_header(const std::string& key, const std::string& val);
    std::string serialize() const;

private:
    static std::string default_status_message(int code);
};

#endif // HTTP_RESPONSE_HPP
