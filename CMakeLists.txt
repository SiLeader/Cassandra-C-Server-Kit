cmake_minimum_required(VERSION 3.15)
project(cassandra_serverkit)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS date_time system)
include_directories(${Boost_INCLUDE_DIRS})

add_library(cqlsvrkit library.cpp library.h include/cqlsvrkit/cqlsvrkit.hpp include/cqlsvrkit/tcp/server.hpp include/cqlsvrkit/tcp/socket.hpp)
target_link_libraries(cqlsvrkit Boost::date_time Boost::system pthread)
