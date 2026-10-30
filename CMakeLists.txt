cmake_minimum_required(VERSION 3.14)
project(cpp_http_server CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -pthread)
endif()

include_directories(include)

set(SERVER_LIB_SOURCES
    src/http_request.cpp
    src/http_response.cpp
    src/router.cpp
    src/thread_pool.cpp
    src/http_server.cpp
    src/store.cpp
    src/controllers.cpp
)

add_library(server_lib STATIC ${SERVER_LIB_SOURCES})
find_package(Threads REQUIRED)
target_link_libraries(server_lib PRIVATE Threads::Threads)

add_executable(cpp_http_server src/main.cpp)
target_link_libraries(cpp_http_server PRIVATE server_lib Threads::Threads)

enable_testing()

add_executable(server_tests
    tests/main_tests.cpp
    tests/test_router.cpp
    tests/test_store.cpp
    tests/test_api.cpp
)
target_link_libraries(server_tests PRIVATE server_lib Threads::Threads)

add_test(NAME AllServerTests COMMAND server_tests)
