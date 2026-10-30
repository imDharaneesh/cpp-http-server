#ifndef USER_HPP
#define USER_HPP

#include "json_utils.hpp"
#include <string>

struct User {
    int id = 0;
    std::string name;
    std::string email;
    std::string role;

    std::string to_json() const {
        return "{\"id\":" + std::to_string(id) +
               ",\"name\":\"" + JsonUtils::escape_string(name) + "\"" +
               ",\"email\":\"" + JsonUtils::escape_string(email) + "\"" +
               ",\"role\":\"" + JsonUtils::escape_string(role) + "\"}";
    }

    static User from_json(const std::string& json_str) {
        User u;
        auto map = JsonUtils::parse_object(json_str);
        if (map.count("id")) u.id = std::stoi(map["id"]);
        if (map.count("name")) u.name = map["name"];
        if (map.count("email")) u.email = map["email"];
        if (map.count("role")) u.role = map["role"];
        return u;
    }
};

#endif // USER_HPP
