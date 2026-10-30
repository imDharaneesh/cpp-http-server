#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "http_request.hpp"
#include "http_response.hpp"
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

using HandlerFunc = std::function<HttpResponse(const HttpRequest&)>;

struct Route {
    std::string method;
    std::string pattern; // e.g. "/api/v1/users/{id}"
    HandlerFunc handler;
    std::vector<std::string> param_names;
};

class Router {
public:
    void add_route(const std::string& method, const std::string& pattern, HandlerFunc handler);
    void get(const std::string& pattern, HandlerFunc handler);
    void post(const std::string& pattern, HandlerFunc handler);
    void put(const std::string& pattern, HandlerFunc handler);
    void del(const std::string& pattern, HandlerFunc handler);

    HttpResponse route(HttpRequest& request) const;

private:
    std::vector<Route> routes_;
    bool match_pattern(const std::string& pattern, const std::string& path, std::unordered_map<std::string, std::string>& params) const;
    static std::vector<std::string> split_path(const std::string& path);
};

#endif // ROUTER_HPP
