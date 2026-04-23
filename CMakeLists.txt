cmake_minimum_required(VERSION 3.15)
project(RouterP2P VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) # Coroutines C++20 sont géniales pour le réseau
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recherche des dépendances installées par Conan
find_package(asio REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

# Liaison des bibliothèques
target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio OpenSSL::SSL)
target_include_directories(${PROJECT_NAME} PUBLIC include)