#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "router.hpp"
#include "thread_pool.hpp"
#include <string>
#include <atomic>

class HttpServer {
public:
    HttpServer(int port, size_t thread_count = 8);
    ~HttpServer();

    void set_router(const Router& router);
    void start();
    void stop();

private:
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    Router router_;
    ThreadPool pool_;

    void handle_client(int client_fd);
};

#endif // HTTP_SERVER_HPP
