#include "http_response.hpp"
#include <sstream>

std::string HttpResponse::default_status_message(int code) {
    switch (code) {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 409: return "Conflict";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

HttpResponse HttpResponse::json(int code, const std::string& json_body) {
    HttpResponse res;
    res.status_code = code;
    res.status_message = default_status_message(code);
    res.headers["Content-Type"] = "application/json";
    res.body = json_body;
    return res;
}

HttpResponse HttpResponse::text(int code, const std::string& text_body) {
    HttpResponse res;
    res.status_code = code;
    res.status_message = default_status_message(code);
    res.headers["Content-Type"] = "text/plain";
    res.body = text_body;
    return res;
}

HttpResponse HttpResponse::status(int code, const std::string& msg) {
    HttpResponse res;
    res.status_code = code;
    res.status_message = msg.empty() ? default_status_message(code) : msg;
    res.body = "";
    return res;
}

HttpResponse& HttpResponse::set_header(const std::string& key, const std::string& val) {
    headers[key] = val;
    return *this;
}

std::string HttpResponse::serialize() const {
    std::ostringstream ss;
    ss << "HTTP/1.1 " << status_code << " " << status_message << "\r\n";

    std::unordered_map<std::string, std::string> final_headers = headers;
    final_headers["Content-Length"] = std::to_string(body.size());

    for (const auto& [k, v] : final_headers) {
        ss << k << ": " << v << "\r\n";
    }
    ss << "\r\n";
    ss << body;
    return ss.str();
}
