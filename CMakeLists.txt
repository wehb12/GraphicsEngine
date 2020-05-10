CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(WillEngine VERSION 0.0)

# Game source files
SET(ENGINE_SOURCES
	Source/main.cpp
)

file(GLOB ENGINE_HEADERS
    # "Source/ThirParty/Include/*.h"
)

# Source grouping for Visual Studio and Xcode
# CMake < 3.8 doesn't support the tree feature, Android Studio's CMake version is older
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0")
	SOURCE_GROUP(WillEngine FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})
ELSE()
    SOURCE_GROUP(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})
ENDIF()

link_directories("${CMAKE_SOURCE_DIR}/Source/Graphics/glfw-3.3.2/build/src/Debug")

# Create an executable target called 'game'
ADD_EXECUTABLE(WillEngine ${ENGINE_SOURCES} ${ENGINE_HEADERS})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)

# find_package(GLM REQUIRED)
# message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# GLAD
add_library(GLAD "Source/Graphics/glad/src/glad.c")

set(LIBS glfw3 opengl32 GLAD)

include_directories(
    "${CMAKE_SOURCE_DIR}/Source/Graphics/glad/include"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/glfw-3.3.2/include"
)

target_link_libraries(${PROJECT_NAME} ${LIBS})
