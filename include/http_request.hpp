#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <unordered_map>

class HttpRequest {
public:
    std::string method;
    std::string path;
    std::string query_string;
    std::string http_version;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> query_params;
    std::unordered_map<std::string, std::string> path_params;
    std::string body;

    static HttpRequest parse(const std::string& raw_request);
    std::string get_header(const std::string& key, const std::string& default_val = "") const;
    std::string get_query_param(const std::string& key, const std::string& default_val = "") const;
    std::string get_path_param(const std::string& key, const std::string& default_val = "") const;

private:
    static void parse_query_params(const std::string& query, std::unordered_map<std::string, std::string>& params);
    static std::string url_decode(const std::string& str);
};

#endif // HTTP_REQUEST_HPP
