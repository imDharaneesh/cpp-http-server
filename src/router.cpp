#include "router.hpp"
#include <sstream>

std::vector<std::string> Router::split_path(const std::string& path) {
    std::vector<std::string> segments;
    std::stringstream ss(path);
    std::string segment;
    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }
    return segments;
}

void Router::add_route(const std::string& method, const std::string& pattern, HandlerFunc handler) {
    Route r;
    r.method = method;
    r.pattern = pattern;
    r.handler = handler;

    std::vector<std::string> segs = split_path(pattern);
    for (const auto& seg : segs) {
        if (seg.front() == '{' && seg.back() == '}') {
            r.param_names.push_back(seg.substr(1, seg.size() - 2));
        }
    }
    routes_.push_back(r);
}

void Router::get(const std::string& pattern, HandlerFunc handler) {
    add_route("GET", pattern, handler);
}

void Router::post(const std::string& pattern, HandlerFunc handler) {
    add_route("POST", pattern, handler);
}

void Router::put(const std::string& pattern, HandlerFunc handler) {
    add_route("PUT", pattern, handler);
}

void Router::del(const std::string& pattern, HandlerFunc handler) {
    add_route("DELETE", pattern, handler);
}

bool Router::match_pattern(const std::string& pattern, const std::string& path, std::unordered_map<std::string, std::string>& params) const {
    std::vector<std::string> pat_segs = split_path(pattern);
    std::vector<std::string> path_segs = split_path(path);

    if (pat_segs.size() != path_segs.size()) return false;

    for (size_t i = 0; i < pat_segs.size(); ++i) {
        if (pat_segs[i].front() == '{' && pat_segs[i].back() == '}') {
            std::string key = pat_segs[i].substr(1, pat_segs[i].size() - 2);
            params[key] = path_segs[i];
        } else if (pat_segs[i] != path_segs[i]) {
            return false;
        }
    }
    return true;
}

HttpResponse Router::route(HttpRequest& request) const {
    bool path_matched = false;
    for (const auto& r : routes_) {
        std::unordered_map<std::string, std::string> params;
        if (match_pattern(r.pattern, request.path, params)) {
            path_matched = true;
            if (r.method == request.method) {
                request.path_params = params;
                return r.handler(request);
            }
        }
    }

    if (path_matched) {
        return HttpResponse::json(405, "{\"error\": \"Method Not Allowed\"}");
    }
    return HttpResponse::json(404, "{\"error\": \"Route Not Found\"}");
}
