#include <iostream>

int main_router_tests();
int main_store_tests();
int main_api_tests();

int main() {
    std::cout << "--- Running Server Unit & Integration Tests ---" << std::endl;
    main_router_tests();
    main_store_tests();
    main_api_tests();
    std::cout << "--- All Tests Passed Successfully ---" << std::endl;
    return 0;
}
