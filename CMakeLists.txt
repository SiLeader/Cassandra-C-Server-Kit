cmake_minimum_required(VERSION 3.15)
project(cassandra_serverkit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Weffc++ -Wmissing-declarations -Wmissing-prototypes -Wmultichar -Wnoexcept -Wold-style-cast -Wold-style-definition -Wpedantic -Wpointer-arith -Wredundant-decls -Wreorder -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-prototypes -Wsuggest-override -Wwrite-strings -Wzero-as-null-pointer-constant")

find_package(Boost REQUIRED COMPONENTS date_time system)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(libcpp17/include)
include_directories(include)

add_library(cqlsvrkit library.cpp library.h include/cqlsvrkit/cqlsvrkit.hpp include/cqlsvrkit/tcp/server.hpp include/cqlsvrkit/tcp/socket.hpp include/cqlsvrkit/tcp/detail/data_convert.hpp include/cqlsvrkit/tcp/detail/byte.hpp include/cqlsvrkit/rpc_base.hpp include/cqlsvrkit/types.hpp include/cqlsvrkit/types/common.hpp include/cqlsvrkit/error.hpp include/cqlsvrkit/detail/convert.hpp src/types.cpp src/rpc_base.cpp)
target_link_libraries(cqlsvrkit Boost::date_time Boost::system pthread)
