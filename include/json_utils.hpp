#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <string>
#include <unordered_map>

class JsonUtils {
public:
    static std::string escape_string(const std::string& str) {
        std::string res;
        for (char c : str) {
            if (c == '"') res += "\\\"";
            else if (c == '\\') res += "\\\\";
            else if (c == '\n') res += "\\n";
            else if (c == '\r') res += "\\r";
            else if (c == '\t') res += "\\t";
            else res += c;
        }
        return res;
    }

    static std::unordered_map<std::string, std::string> parse_object(const std::string& json_str) {
        std::unordered_map<std::string, std::string> map;
        if (json_str.empty()) return map;

        size_t start = json_str.find('{');
        size_t end = json_str.rfind('}');
        if (start == std::string::npos || end == std::string::npos || start >= end) return map;

        std::string content = json_str.substr(start + 1, end - start - 1);

        bool in_quotes = false;
        std::string current_key;
        std::string current_val;
        bool parsing_val = false;

        for (size_t i = 0; i < content.size(); ++i) {
            char c = content[i];
            if (c == '"' && (i == 0 || content[i-1] != '\\')) {
                in_quotes = !in_quotes;
                continue;
            }
            if (!in_quotes) {
                if (c == ':') {
                    parsing_val = true;
                    continue;
                }
                if (c == ',') {
                    trim(current_key);
                    trim(current_val);
                    if (!current_key.empty()) map[current_key] = current_val;
                    current_key.clear();
                    current_val.clear();
                    parsing_val = false;
                    continue;
                }
            }
            if (parsing_val) {
                current_val += c;
            } else {
                current_key += c;
            }
        }
        trim(current_key);
        trim(current_val);
        if (!current_key.empty()) map[current_key] = current_val;

        return map;
    }

private:
    static void trim(std::string& s) {
        size_t first = s.find_first_not_of(" \t\n\r\"");
        if (first == std::string::npos) {
            s = "";
            return;
        }
        size_t last = s.find_last_not_of(" \t\n\r\"");
        s = s.substr(first, (last - first + 1));
    }
};

#endif // JSON_UTILS_HPP
