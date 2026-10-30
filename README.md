# Modern C++ HTTP Server & REST API

A multi-threaded C++17 HTTP REST API Server built from scratch using Linux sockets, standard threads, and an in-memory data store.

## Features

- **Multi-threaded Architecture**: Efficient `ThreadPool` handling concurrent HTTP requests.
- **Dynamic Routing & Path Parameters**: Route registration with parameter extraction (e.g. `/api/v1/users/{id}`).
- **JSON REST API Endpoints**: Full CRUD support for `User` and `Item` resources, system status, and utility endpoints.
- **Thread-safe In-Memory Store**: Safe multi-threaded data mutations protected by `std::mutex`.
- **Comprehensive Test Suite**: Automated unit and integration tests covering HTTP protocol parsing, routing, data persistence, and live server endpoints.

## API Endpoints

### System & Health
- `GET /api/v1/health` - Check server health status.
- `GET /api/v1/info` - Get server build and version information.
- `GET /api/v1/stats` - Get current count of users and items in memory.

### Users Management
- `GET /api/v1/users` - Retrieve all users (supports `?role=Admin` query filter).
- `GET /api/v1/users/{id}` - Retrieve a single user by ID.
- `POST /api/v1/users` - Create a new user (`{"name":"Alice","email":"alice@example.com","role":"Admin"}`).
- `PUT /api/v1/users/{id}` - Update an existing user.
- `DELETE /api/v1/users/{id}` - Delete a user.

### Items Management
- `GET /api/v1/items` - Retrieve all inventory items.
- `GET /api/v1/items/{id}` - Retrieve an item by ID.
- `POST /api/v1/items` - Create a new item (`{"name":"Laptop","price":1299.99,"quantity":10}`).
- `PUT /api/v1/items/{id}` - Update an item.
- `DELETE /api/v1/items/{id}` - Delete an item.

### Utility
- `POST /api/v1/echo` - Echo back request metadata and payload.

## Building and Running

### Prerequisites
- C++17 compiler (GCC 7+ or Clang 6+)
- CMake 3.14+
- Make / Ninja

### Build
```bash
cmake -B build
cmake --build build
```

### Run Server
```bash
./build/cpp_http_server
```

### Run Tests
```bash
./build/server_tests
# or using ctest
ctest --test-dir build --output-on-failure
```

## Example cURL Requests

```bash
# Health check
curl http://localhost:8080/api/v1/health

# Create user
curl -X POST http://localhost:8080/api/v1/users \
  -H "Content-Type: application/json" \
  -d '{"name":"John Doe","email":"john@example.com","role":"User"}'

# List users
curl http://localhost:8080/api/v1/users

# Get user by ID
curl http://localhost:8080/api/v1/users/1
```
