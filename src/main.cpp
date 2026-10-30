#include "http_server.hpp"
#include "router.hpp"
#include "controllers.hpp"
#include <iostream>

int main() {
    int port = 8080;

    Router router;
    Controllers::register_routes(router);

    HttpServer server(port, 8);
    server.set_router(router);

    std::cout << "Starting C++ HTTP Server on port " << port << "..." << std::endl;
    server.start();

    return 0;
}
