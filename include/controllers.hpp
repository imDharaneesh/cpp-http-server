#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP

#include "router.hpp"

namespace Controllers {
    void register_routes(Router& router);

    // Health and System Info
    HttpResponse get_health(const HttpRequest& req);
    HttpResponse get_info(const HttpRequest& req);
    HttpResponse get_stats(const HttpRequest& req);

    // User CRUD
    HttpResponse get_users(const HttpRequest& req);
    HttpResponse get_user(const HttpRequest& req);
    HttpResponse create_user(const HttpRequest& req);
    HttpResponse update_user(const HttpRequest& req);
    HttpResponse delete_user(const HttpRequest& req);

    // Item CRUD
    HttpResponse get_items(const HttpRequest& req);
    HttpResponse get_item(const HttpRequest& req);
    HttpResponse create_item(const HttpRequest& req);
    HttpResponse update_item(const HttpRequest& req);
    HttpResponse delete_item(const HttpRequest& req);

    // Utility
    HttpResponse echo(const HttpRequest& req);
}

#endif // CONTROLLERS_HPP
