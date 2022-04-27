cmake_minimum_required(VERSION 3.10)

project(SwarmSimulator)

include_directories(eigen3)

if(UNIX)
    find_package(gazebo REQUIRED)
    include_directories(${GAZEBO_INCLUDE_DIRS})
    link_directories(${GAZEBO_LIBRARY_DIRS})
    list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")
endif()

include_directories(osk/include include)
set(OSKSOURCES osk/src/sim.cpp osk/src/block.cpp osk/src/state.cpp)
set(SRC src/main.cpp src/simplerover.cpp src/libAP_JSON.cpp
    src/SocketExample.cpp src/genericmultirotor.cpp src/p_utils.cpp)

add_library(CMDPlugin SHARED src/CMDPlugin.cc src/SocketExample.cpp)
target_link_libraries(CMDPlugin ${GAZEBO_LIBRARIES})

add_executable(PhysicsEngine ${SRC} ${OSKSOURCES})
