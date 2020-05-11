CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(WillEngine VERSION 0.0)

# Game source files
FILE(GLOB_RECURSE ENGINE_SOURCES
    "Source/*.cpp"
)

# Game header files
FILE(GLOB_RECURSE ENGINE_HEADERS
    "Source/*.h"
)

# Source grouping for Visual Studio and Xcode
# CMake < 3.8 doesn't support the tree feature, Android Studio's CMake version is older
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0")
	SOURCE_GROUP(WillEngine FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})
ELSE()
    SOURCE_GROUP(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})
ENDIF()

LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/Source/Graphics/ThirdParty/glfw-3.3.2/build/src/Debug")

# Create an executable target called 'game'
ADD_EXECUTABLE(WillEngine WIN32 ${ENGINE_SOURCES} ${ENGINE_HEADERS})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    "Source/"
    "Source/Engine/"
)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

FIND_PACKAGE(OpenGL REQUIRED)

# FIND_PACKAGE(GLM REQUIRED)
# MESSAGE(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

FIND_PACKAGE(GLFW3 REQUIRED)
MESSAGE(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# GLAD
ADD_LIBRARY(GLAD "Source/Graphics/ThirdParty/glad/src/glad.c")

SET(LIBS glfw3 opengl32 GLAD)

INCLUDE_DIRECTORIES(
    "${CMAKE_SOURCE_DIR}/Source/Graphics/ThirdParty/glad/include"
    "${CMAKE_SOURCE_DIR}/Source/Graphics/ThirdParty/glfw-3.3.2/include"
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
