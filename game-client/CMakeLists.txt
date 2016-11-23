#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)


# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(loque)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories("${PROJECT_BINARY_DIR}")

# Define sources and executable
set(EXECUTABLE_NAME "loque")

set( source
         src/entity.cpp
         src/Animation.cpp
         src/AnimatedSprite.cpp
         src/DebugDraw.cpp
         src/resource.cpp
         src/world.cpp
         src/player.cpp
         src/map.cpp
         src/playerstate.cpp
         src/contactlistener.cpp
         src/physicalworld.cpp
         src/main.cpp )

add_executable(${EXECUTABLE_NAME} ${source})


# Detect and add SFML
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules" )
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(OpenGL REQUIRED)
find_package(TMXLITE REQUIRED)
find_package(Box2D REQUIRED)
find_package(GLM REQUIRED)

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR}
                      ${OPENGL_INCLUDE_DIR}
                      ${GLM_INCLUDE_DIR}
                      ${BOX2D_INCLUDE_DIR}
                      ${TMXLITE_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES}
                                           ${OPENGL_LIBRARIES}
                                           ${BOX2D_LIBRARIES}
                                           ${TMXLITE_LIBRARIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
