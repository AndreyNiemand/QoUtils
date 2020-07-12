CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(test_QoUtils LANGUAGES CXX)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

file(GLOB QoUtil_FILES "../*.hpp")
file(GLOB TESTS_FILES "*.cpp" module.hpp)

add_executable(${PROJECT_NAME} ${TESTS_FILES} ${QoUtil_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "../" ${Boost_INCLUDE_DIR})

