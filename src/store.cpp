#include "store.hpp"
#include <algorithm>

Store::Store() {
    // Seed initial sample data
    create_user("Alice Smith", "alice@example.com", "Admin");
    create_user("Bob Jones", "bob@example.com", "User");
    create_item("Laptop", 1299.99, 10);
    create_item("Wireless Mouse", 29.99, 50);
}

void Store::reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    users_.clear();
    items_.clear();
    next_user_id_ = 1;
    next_item_id_ = 1;
}

size_t Store::user_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return users_.size();
}

size_t Store::item_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.size();
}

std::vector<User> Store::get_all_users() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return users_;
}

std::optional<User> Store::get_user_by_id(int id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& u : users_) {
        if (u.id == id) return u;
    }
    return std::nullopt;
}

User Store::create_user(const std::string& name, const std::string& email, const std::string& role) {
    std::lock_guard<std::mutex> lock(mutex_);
    User u;
    u.id = next_user_id_++;
    u.name = name;
    u.email = email;
    u.role = role.empty() ? "User" : role;
    users_.push_back(u);
    return u;
}

bool Store::update_user(int id, const std::string& name, const std::string& email, const std::string& role) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& u : users_) {
        if (u.id == id) {
            if (!name.empty()) u.name = name;
            if (!email.empty()) u.email = email;
            if (!role.empty()) u.role = role;
            return true;
        }
    }
    return false;
}

bool Store::delete_user(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::remove_if(users_.begin(), users_.end(), [id](const User& u) {
        return u.id == id;
    });
    if (it != users_.end()) {
        users_.erase(it, users_.end());
        return true;
    }
    return false;
}

std::vector<Item> Store::get_all_items() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return items_;
}

std::optional<Item> Store::get_item_by_id(int id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& item : items_) {
        if (item.id == id) return item;
    }
    return std::nullopt;
}

Item Store::create_item(const std::string& name, double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex_);
    Item item;
    item.id = next_item_id_++;
    item.name = name;
    item.price = price;
    item.quantity = quantity;
    items_.push_back(item);
    return item;
}

bool Store::update_item(int id, const std::string& name, double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& item : items_) {
        if (item.id == id) {
            if (!name.empty()) item.name = name;
            if (price >= 0) item.price = price;
            if (quantity >= 0) item.quantity = quantity;
            return true;
        }
    }
    return false;
}

bool Store::delete_item(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::remove_if(items_.begin(), items_.end(), [id](const Item& item) {
        return item.id == id;
    });
    if (it != items_.end()) {
        items_.erase(it, items_.end());
        return true;
    }
    return false;
}
