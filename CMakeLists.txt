cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(myCQ)

find_package(Boost COMPONENTS program_options filesystem REQUIRED)
find_package(Threads REQUIRED)

# Setup testing - test in subdirectory tests
enable_testing() # tell cmake that you want it to generate test scripts --> "enable ctest"
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})  

add_library(custom_net_lib INTERFACE) # The INTERFACE keyword indicates, that the libary is a header based library
target_include_directories(custom_net_lib INTERFACE include/custom_net_lib/ ${Boost_INCLUDE_DIRS})
target_link_libraries(custom_net_lib INTERFACE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(myCQ_client src/main_client.cpp)
target_link_libraries(myCQ_client PRIVATE custom_net_lib ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(myCQ_client PRIVATE include ${Boost_INCLUDE_DIRS})

add_executable(myCQ_server src/main_server.cpp)
target_link_libraries(myCQ_server PRIVATE custom_net_lib ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(myCQ_server PRIVATE include ${Boost_INCLUDE_DIRS})

# add unit tests
add_subdirectory(tests)