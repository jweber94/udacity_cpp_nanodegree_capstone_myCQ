cmake_minimum_required(VERSION 3.11.4)
project(test_custom_networking)

set(CMAKE_CXX_STANDARD 11) # This is very important for GTest to run!

find_package(Boost REQUIRED)

# create an executable that describes the tests
add_executable(unit_tests test_msg_serialization.cpp)

# link gtest against the executable that contains the tests
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/custom_net_lib ${Boost_INCLUDE_DIRS})
target_link_libraries(unit_tests PUBLIC custom_net_lib gtest gtest_main pthread ${Boost_LIBRARIES}) 

# define for cmake that this is a test
add_test(NAME unit_tests COMMAND unit_tests)