#include "router.hpp"
#include <iostream>
#include <cassert>

void test_router_matching() {
    Router router;
    bool handler_called = false;
    std::string captured_id;

    router.get("/api/v1/users/{id}", [&](const HttpRequest& req) {
        handler_called = true;
        captured_id = req.get_path_param("id");
        return HttpResponse::json(200, "{\"ok\":true}");
    });

    HttpRequest req = HttpRequest::parse("GET /api/v1/users/42 HTTP/1.1\r\nHost: localhost\r\n\r\n");
    HttpResponse res = router.route(req);

    assert(handler_called);
    assert(captured_id == "42");
    assert(res.status_code == 200);
    std::cout << "[PASS] test_router_matching" << std::endl;
}

void test_router_404_405() {
    Router router;
    router.get("/api/v1/test", [](const HttpRequest&) {
        return HttpResponse::json(200, "{}");
    });

    HttpRequest req404 = HttpRequest::parse("GET /api/v1/unknown HTTP/1.1\r\n\r\n");
    HttpResponse res404 = router.route(req404);
    assert(res404.status_code == 404);

    HttpRequest req405 = HttpRequest::parse("POST /api/v1/test HTTP/1.1\r\n\r\n");
    HttpResponse res405 = router.route(req405);
    assert(res405.status_code == 405);

    std::cout << "[PASS] test_router_404_405" << std::endl;
}

int main_router_tests() {
    test_router_matching();
    test_router_404_405();
    return 0;
}
