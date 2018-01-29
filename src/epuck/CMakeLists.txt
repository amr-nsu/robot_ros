project(epuck)
cmake_minimum_required(VERSION 2.8)

set(CATKIN_ENABLE_TESTING OFF)  # disable gtest
find_package(catkin REQUIRED COMPONENTS roscpp)
include_directories(${catkin_INCLUDE_DIRS})
catkin_package()

add_definitions(-DUSE_ROS)

# ---

set(SOURCES
    src/EPuck.hpp
    src/EPuck.cpp
    src/main.cpp
)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")                    # max debug info
endif()


find_package(Qt5Widgets)
find_package(Qt5SerialPort REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
qt5_wrap_ui(FORMS_HEADERS ${FORMS})
qt5_add_resources(RESOURCES_SOURCES ${RESOURCES})

find_package (Eigen3 REQUIRED NO_MODULE)
include_directories(${Eigen_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
               ${SOURCES}
)

qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME}
                      ${catkin_LIBRARIES}
                      Qt5::SerialPort
)
