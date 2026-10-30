#ifndef ITEM_HPP
#define ITEM_HPP

#include "json_utils.hpp"
#include <string>

struct Item {
    int id = 0;
    std::string name;
    double price = 0.0;
    int quantity = 0;

    std::string to_json() const {
        return "{\"id\":" + std::to_string(id) +
               ",\"name\":\"" + JsonUtils::escape_string(name) + "\"" +
               ",\"price\":" + std::to_string(price) +
               ",\"quantity\":" + std::to_string(quantity) + "}";
    }

    static Item from_json(const std::string& json_str) {
        Item item;
        auto map = JsonUtils::parse_object(json_str);
        if (map.count("id")) item.id = std::stoi(map["id"]);
        if (map.count("name")) item.name = map["name"];
        if (map.count("price")) item.price = std::stod(map["price"]);
        if (map.count("quantity")) item.quantity = std::stoi(map["quantity"]);
        return item;
    }
};

#endif // ITEM_HPP
