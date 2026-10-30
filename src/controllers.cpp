#include "controllers.hpp"
#include "store.hpp"
#include "json_utils.hpp"
#include <sstream>

namespace Controllers {

void register_routes(Router& router) {
    router.get("/api/v1/health", get_health);
    router.get("/api/v1/info", get_info);
    router.get("/api/v1/stats", get_stats);

    router.get("/api/v1/users", get_users);
    router.get("/api/v1/users/{id}", get_user);
    router.post("/api/v1/users", create_user);
    router.put("/api/v1/users/{id}", update_user);
    router.del("/api/v1/users/{id}", delete_user);

    router.get("/api/v1/items", get_items);
    router.get("/api/v1/items/{id}", get_item);
    router.post("/api/v1/items", create_item);
    router.put("/api/v1/items/{id}", update_item);
    router.del("/api/v1/items/{id}", delete_item);

    router.post("/api/v1/echo", echo);
}

HttpResponse get_health(const HttpRequest& /*req*/) {
    return HttpResponse::json(200, "{\"status\":\"UP\",\"service\":\"cpp-http-server\"}");
}

HttpResponse get_info(const HttpRequest& /*req*/) {
    return HttpResponse::json(200, "{\"name\":\"Modern C++ HTTP REST API Server\",\"version\":\"1.0.0\",\"cpp_standard\":\"C++17\"}");
}

HttpResponse get_stats(const HttpRequest& /*req*/) {
    auto& store = Store::get_instance();
    std::string json = "{\"users_count\":" + std::to_string(store.user_count()) +
                       ",\"items_count\":" + std::to_string(store.item_count()) + "}";
    return HttpResponse::json(200, json);
}

HttpResponse get_users(const HttpRequest& req) {
    auto users = Store::get_instance().get_all_users();
    std::string role_filter = req.get_query_param("role");

    std::ostringstream ss;
    ss << "[";
    bool first = true;
    for (const auto& u : users) {
        if (!role_filter.empty() && u.role != role_filter) continue;
        if (!first) ss << ",";
        ss << u.to_json();
        first = false;
    }
    ss << "]";
    return HttpResponse::json(200, ss.str());
}

HttpResponse get_user(const HttpRequest& req) {
    std::string id_str = req.get_path_param("id");
    int id = 0;
    try {
        id = std::stoi(id_str);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid user ID\"}");
    }

    auto user = Store::get_instance().get_user_by_id(id);
    if (user.has_value()) {
        return HttpResponse::json(200, user->to_json());
    }
    return HttpResponse::json(404, "{\"error\":\"User not found\"}");
}

HttpResponse create_user(const HttpRequest& req) {
    auto parsed = JsonUtils::parse_object(req.body);
    if (!parsed.count("name") || !parsed.count("email")) {
        return HttpResponse::json(400, "{\"error\":\"Missing required fields 'name' and 'email'\"}");
    }

    std::string name = parsed["name"];
    std::string email = parsed["email"];
    std::string role = parsed.count("role") ? parsed["role"] : "User";

    User new_user = Store::get_instance().create_user(name, email, role);
    return HttpResponse::json(201, new_user.to_json());
}

HttpResponse update_user(const HttpRequest& req) {
    std::string id_str = req.get_path_param("id");
    int id = 0;
    try {
        id = std::stoi(id_str);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid user ID\"}");
    }

    auto parsed = JsonUtils::parse_object(req.body);
    std::string name = parsed.count("name") ? parsed["name"] : "";
    std::string email = parsed.count("email") ? parsed["email"] : "";
    std::string role = parsed.count("role") ? parsed["role"] : "";

    bool updated = Store::get_instance().update_user(id, name, email, role);
    if (updated) {
        auto user = Store::get_instance().get_user_by_id(id);
        return HttpResponse::json(200, user->to_json());
    }
    return HttpResponse::json(404, "{\"error\":\"User not found\"}");
}

HttpResponse delete_user(const HttpRequest& req) {
    std::string id_str = req.get_path_param("id");
    int id = 0;
    try {
        id = std::stoi(id_str);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid user ID\"}");
    }

    bool deleted = Store::get_instance().delete_user(id);
    if (deleted) {
        return HttpResponse::json(200, "{\"message\":\"User deleted successfully\"}");
    }
    return HttpResponse::json(404, "{\"error\":\"User not found\"}");
}

HttpResponse get_items(const HttpRequest& /*req*/) {
    auto items = Store::get_instance().get_all_items();
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) ss << ",";
        ss << items[i].to_json();
    }
    ss << "]";
    return HttpResponse::json(200, ss.str());
}

HttpResponse get_item(const HttpRequest& req) {
    std::string id_str = req.get_path_param("id");
    int id = 0;
    try {
        id = std::stoi(id_str);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid item ID\"}");
    }

    auto item = Store::get_instance().get_item_by_id(id);
    if (item.has_value()) {
        return HttpResponse::json(200, item->to_json());
    }
    return HttpResponse::json(404, "{\"error\":\"Item not found\"}");
}

HttpResponse create_item(const HttpRequest& req) {
    auto parsed = JsonUtils::parse_object(req.body);
    if (!parsed.count("name") || !parsed.count("price")) {
        return HttpResponse::json(400, "{\"error\":\"Missing required fields 'name' and 'price'\"}");
    }

    std::string name = parsed["name"];
    double price = 0.0;
    int quantity = 0;
    try {
        price = std::stod(parsed["price"]);
        if (parsed.count("quantity")) quantity = std::stoi(parsed["quantity"]);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid numeric parameters\"}");
    }

    Item new_item = Store::get_instance().create_item(name, price, quantity);
    return HttpResponse::json(201, new_item.to_json());
}

HttpResponse update_item(const HttpRequest& req) {
    std::string id_str = req.get_path_param("id");
    int id = 0;
    try {
        id = std::stoi(id_str);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid item ID\"}");
    }

    auto parsed = JsonUtils::parse_object(req.body);
    std::string name = parsed.count("name") ? parsed["name"] : "";
    double price = parsed.count("price") ? std::stod(parsed["price"]) : -1.0;
    int quantity = parsed.count("quantity") ? std::stoi(parsed["quantity"]) : -1;

    bool updated = Store::get_instance().update_item(id, name, price, quantity);
    if (updated) {
        auto item = Store::get_instance().get_item_by_id(id);
        return HttpResponse::json(200, item->to_json());
    }
    return HttpResponse::json(404, "{\"error\":\"Item not found\"}");
}

HttpResponse delete_item(const HttpRequest& req) {
    std::string id_str = req.get_path_param("id");
    int id = 0;
    try {
        id = std::stoi(id_str);
    } catch (...) {
        return HttpResponse::json(400, "{\"error\":\"Invalid item ID\"}");
    }

    bool deleted = Store::get_instance().delete_item(id);
    if (deleted) {
        return HttpResponse::json(200, "{\"message\":\"Item deleted successfully\"}");
    }
    return HttpResponse::json(404, "{\"error\":\"Item not found\"}");
}

HttpResponse echo(const HttpRequest& req) {
    std::ostringstream ss;
    ss << "{\"method\":\"" << JsonUtils::escape_string(req.method) << "\""
       << ",\"path\":\"" << JsonUtils::escape_string(req.path) << "\""
       << ",\"body\":\"" << JsonUtils::escape_string(req.body) << "\"}";
    return HttpResponse::json(200, ss.str());
}

} // namespace Controllers
