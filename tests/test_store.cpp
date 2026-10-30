#include "store.hpp"
#include <iostream>
#include <cassert>

void test_store_user_crud() {
    Store& store = Store::get_instance();
    store.reset();

    assert(store.user_count() == 0);

    User u = store.create_user("Alice", "alice@test.com", "Admin");
    assert(u.id == 1);
    assert(store.user_count() == 1);

    auto found = store.get_user_by_id(1);
    assert(found.has_value());
    assert(found->name == "Alice");

    bool updated = store.update_user(1, "Alice Updated", "", "");
    assert(updated);
    auto updated_u = store.get_user_by_id(1);
    assert(updated_u->name == "Alice Updated");
    assert(updated_u->email == "alice@test.com");

    bool deleted = store.delete_user(1);
    assert(deleted);
    assert(store.user_count() == 0);

    std::cout << "[PASS] test_store_user_crud" << std::endl;
}

void test_store_item_crud() {
    Store& store = Store::get_instance();
    store.reset();

    Item item = store.create_item("Phone", 699.99, 5);
    assert(item.id == 1);

    auto found = store.get_item_by_id(1);
    assert(found.has_value());
    assert(found->name == "Phone");

    bool updated = store.update_item(1, "", 649.99, 8);
    assert(updated);
    auto updated_item = store.get_item_by_id(1);
    assert(updated_item->price == 649.99);
    assert(updated_item->quantity == 8);

    bool deleted = store.delete_item(1);
    assert(deleted);
    assert(store.item_count() == 0);

    std::cout << "[PASS] test_store_item_crud" << std::endl;
}

int main_store_tests() {
    test_store_user_crud();
    test_store_item_crud();
    return 0;
}
