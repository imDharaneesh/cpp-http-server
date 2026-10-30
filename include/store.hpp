#ifndef STORE_HPP
#define STORE_HPP

#include "user.hpp"
#include "item.hpp"
#include <vector>
#include <optional>
#include <mutex>

class Store {
public:
    static Store& get_instance() {
        static Store instance;
        return instance;
    }

    // User Operations
    std::vector<User> get_all_users() const;
    std::optional<User> get_user_by_id(int id) const;
    User create_user(const std::string& name, const std::string& email, const std::string& role);
    bool update_user(int id, const std::string& name, const std::string& email, const std::string& role);
    bool delete_user(int id);

    // Item Operations
    std::vector<Item> get_all_items() const;
    std::optional<Item> get_item_by_id(int id) const;
    Item create_item(const std::string& name, double price, int quantity);
    bool update_item(int id, const std::string& name, double price, int quantity);
    bool delete_item(int id);

    size_t user_count() const;
    size_t item_count() const;
    void reset();

private:
    Store();
    mutable std::mutex mutex_;
    int next_user_id_ = 1;
    int next_item_id_ = 1;
    std::vector<User> users_;
    std::vector<Item> items_;
};

#endif // STORE_HPP
