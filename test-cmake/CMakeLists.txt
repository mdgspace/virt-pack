cmake_minimum_required(VERSION 3.10)
project(MyPkgConfigApp C)

# Ask pkg-config to find the packages (replace with the ones you want)
find_package(PkgConfig REQUIRED)

# Example: link to libssl and libcurl via pkg-config
pkg_check_modules(SSL REQUIRED openssl)
pkg_check_modules(CURL REQUIRED libcurl)

# Define the executable
add_executable(my_app main.c)

# Add include directories from pkg-config
target_include_directories(my_app PRIVATE ${SSL_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

# Add linker flags and libraries from pkg-config
target_link_libraries(my_app PRIVATE ${SSL_LIBRARIES} ${CURL_LIBRARIES})
