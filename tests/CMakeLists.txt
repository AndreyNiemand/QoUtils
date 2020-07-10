CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(QoUtils LANGUAGES CXX)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../")
include_directories(${Boost_INCLUDE_DIR})

SET(HEADERS "../map.hpp"
            "../container-viewer.hpp"
    )

SET(SOURCES
        test_map.cpp
        test_container-viewer.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
