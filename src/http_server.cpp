#include "http_server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

HttpServer::HttpServer(int port, size_t thread_count)
    : port_(port), server_fd_(-1), running_(false), pool_(thread_count) {}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::set_router(const Router& router) {
    router_ = router;
}

void HttpServer::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        perror("Socket creation failed");
        return;
    }

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        close(server_fd_);
        return;
    }

    // Set 1-second timeout on accept so loop can check running_ flag when stopping
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(server_fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd_);
        return;
    }

    if (listen(server_fd_, 128) < 0) {
        perror("Listen failed");
        close(server_fd_);
        return;
    }

    running_ = true;
    std::cout << "Server listening on port " << port_ << "..." << std::endl;

    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) {
            if (!running_) break;
            continue;
        }

        pool_.enqueue([this, client_fd]() {
            this->handle_client(client_fd);
        });
    }
}

void HttpServer::stop() {
    if (running_) {
        running_ = false;
        if (server_fd_ >= 0) {
            close(server_fd_);
            server_fd_ = -1;
        }
    }
}

void HttpServer::handle_client(int client_fd) {
    char buffer[8192];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::string raw_req(buffer, bytes_read);

        HttpRequest req = HttpRequest::parse(raw_req);
        HttpResponse res = router_.route(req);

        std::string raw_res = res.serialize();
        write(client_fd, raw_res.c_str(), raw_res.size());
    }
    close(client_fd);
}
