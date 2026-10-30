#include "http_server.hpp"
#include "controllers.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

std::string send_http_request(int port, const std::string& request_str) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int conn = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    assert(conn >= 0);

    write(sock, request_str.c_str(), request_str.size());

    char buffer[4096];
    ssize_t bytes = read(sock, buffer, sizeof(buffer) - 1);
    close(sock);

    if (bytes > 0) {
        buffer[bytes] = '\0';
        return std::string(buffer, bytes);
    }
    return "";
}

void test_api_integration() {
    int port = 8089;
    Router router;
    Controllers::register_routes(router);

    HttpServer server(port, 4);
    server.set_router(router);

    std::thread server_thread([&server]() {
        server.start();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Test GET /api/v1/health
    std::string res_health = send_http_request(port, "GET /api/v1/health HTTP/1.1\r\nHost: localhost\r\n\r\n");
    assert(res_health.find("200 OK") != std::string::npos);
    assert(res_health.find("\"status\":\"UP\"") != std::string::npos);

    // Test POST /api/v1/users
    std::string post_user_req = "POST /api/v1/users HTTP/1.1\r\n"
                                "Host: localhost\r\n"
                                "Content-Type: application/json\r\n"
                                "\r\n"
                                "{\"name\":\"Charlie\",\"email\":\"charlie@test.com\",\"role\":\"Developer\"}";
    std::string res_post_user = send_http_request(port, post_user_req);
    assert(res_post_user.find("201 Created") != std::string::npos);
    assert(res_post_user.find("\"name\":\"Charlie\"") != std::string::npos);

    // Test GET /api/v1/users
    std::string res_get_users = send_http_request(port, "GET /api/v1/users HTTP/1.1\r\nHost: localhost\r\n\r\n");
    assert(res_get_users.find("200 OK") != std::string::npos);
    assert(res_get_users.find("Charlie") != std::string::npos);

    server.stop();
    if (server_thread.joinable()) {
        server_thread.join();
    }

    std::cout << "[PASS] test_api_integration" << std::endl;
}

int main_api_tests() {
    test_api_integration();
    return 0;
}
