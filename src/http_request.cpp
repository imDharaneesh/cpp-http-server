#include "http_request.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string HttpRequest::url_decode(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.size()) {
            int hex_val = 0;
            std::stringstream ss;
            ss << std::hex << str.substr(i + 1, 2);
            ss >> hex_val;
            result += static_cast<char>(hex_val);
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

void HttpRequest::parse_query_params(const std::string& query, std::unordered_map<std::string, std::string>& params) {
    if (query.empty()) return;
    std::stringstream ss(query);
    std::string pair;
    while (std::getline(ss, pair, '&')) {
        if (pair.empty()) continue;
        size_t eq = pair.find('=');
        if (eq != std::string::npos) {
            std::string key = url_decode(pair.substr(0, eq));
            std::string val = url_decode(pair.substr(eq + 1));
            params[key] = val;
        } else {
            params[url_decode(pair)] = "";
        }
    }
}

HttpRequest HttpRequest::parse(const std::string& raw_request) {
    HttpRequest req;
    std::istringstream stream(raw_request);
    std::string line;

    if (!std::getline(stream, line)) return req;
    if (!line.empty() && line.back() == '\r') line.pop_back();

    std::istringstream line_stream(line);
    std::string full_path;
    line_stream >> req.method >> full_path >> req.http_version;

    size_t qmark = full_path.find('?');
    if (qmark != std::string::npos) {
        req.path = full_path.substr(0, qmark);
        req.query_string = full_path.substr(qmark + 1);
        parse_query_params(req.query_string, req.query_params);
    } else {
        req.path = full_path;
    }

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break; // End of headers

        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);

            // Trim leading space in value
            size_t start = value.find_first_not_of(" \t");
            if (start != std::string::npos) {
                value = value.substr(start);
            } else {
                value = "";
            }

            // Lowercase header key for case-insensitive lookup
            std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
            req.headers[key] = value;
        }
    }

    // Read remaining body if present
    std::string body_content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    req.body = body_content;

    return req;
}

std::string HttpRequest::get_header(const std::string& key, const std::string& default_val) const {
    std::string lower_key = key;
    std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(), [](unsigned char c){ return std::tolower(c); });
    auto it = headers.find(lower_key);
    if (it != headers.end()) return it->second;
    return default_val;
}

std::string HttpRequest::get_query_param(const std::string& key, const std::string& default_val) const {
    auto it = query_params.find(key);
    if (it != query_params.end()) return it->second;
    return default_val;
}

std::string HttpRequest::get_path_param(const std::string& key, const std::string& default_val) const {
    auto it = path_params.find(key);
    if (it != path_params.end()) return it->second;
    return default_val;
}
