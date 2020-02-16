cmake_minimum_required(VERSION 3.15)
project(cassandra_serverkit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++")

find_package(Boost REQUIRED COMPONENTS date_time system)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(libcpp17/include)
include_directories(include)

add_library(cqlsvrkit
        include/cqlsvrkit/cqlsvrkit.hpp
        include/cqlsvrkit/tcp/server.hpp
        include/cqlsvrkit/tcp/socket.hpp
        include/cqlsvrkit/tcp/detail/data_convert.hpp
        include/cqlsvrkit/tcp/detail/byte.hpp include/cqlsvrkit/rpc_base.hpp
        include/cqlsvrkit/types.hpp
        include/cqlsvrkit/types/common.hpp
        include/cqlsvrkit/error.hpp
        include/cqlsvrkit/detail/convert.hpp
        src/rpc_base.cpp
        src/convert.cpp
        include/cqlsvrkit/rpc_v4.hpp
        src/rpc_v4.cpp include/cqlsvrkit/types/startup.hpp include/cqlsvrkit/types/header.hpp include/cqlsvrkit/types/query.hpp include/cqlsvrkit/types/result.hpp include/cqlsvrkit/types/supported.hpp src/types_query.cpp include/cqlsvrkit/types/value.hpp src/types_value.cpp src/types_supported.cpp src/types_result.cpp include/cqlsvrkit/types/result/rows.hpp include/cqlsvrkit/types/option.hpp)
target_link_libraries(cqlsvrkit Boost::date_time Boost::system pthread)

add_executable(sample-v4
        sample/sample.cpp)
target_link_libraries(sample-v4 cqlsvrkit)
