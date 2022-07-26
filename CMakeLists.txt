CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(WillEngine VERSION 0.0)

# Game source files
FILE(GLOB_RECURSE ENGINE_SOURCES
    "Source/*.cpp"
)

LIST(FILTER ENGINE_SOURCES EXCLUDE REGEX
    "Source/ThirdParty/.*"
)

# Game header files
FILE(GLOB_RECURSE ENGINE_HEADERS
    "Source/*.h"
)

LIST(FILTER ENGINE_HEADERS EXCLUDE REGEX
    "Source/ThirdParty/.*"
)

# Add stb_image
LIST(APPEND ENGINE_SOURCES
    "Source/ThirdParty/Graphics/stb_image/stb_image.cpp"
)
LIST(APPEND ENGINE_HEADERS
    "Source/ThirdParty/Graphics/stb_image/stb_image.h"
)


# Source grouping for Visual Studio and Xcode
# CMake < 3.8 doesn't support the tree feature, Android Studio's CMake version is older
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0")
	SOURCE_GROUP(WillEngine FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})
ELSE()
    SOURCE_GROUP(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})
ENDIF()

LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics/glfw-3.3.2/build/src/Debug")

# Create an executable target called 'game'
ADD_EXECUTABLE(WillEngine WIN32 ${ENGINE_SOURCES} ${ENGINE_HEADERS})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    "Source/"
    "Source/Engine/"
)

TARGET_COMPILE_DEFINITIONS(WillEngine PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/Source/Graphics/Shaders/")
TARGET_COMPILE_DEFINITIONS(WillEngine PRIVATE CONTENT_DIR="${CMAKE_SOURCE_DIR}/Content/")

IF (WIN32)
    TARGET_COMPILE_DEFINITIONS(WillEngine PRIVATE PLATFORM_WINDOWS=1)
    TARGET_COMPILE_DEFINITIONS(WillEngine PRIVATE PLATFORM_APPLE=0)
ELSEIF (APPLE)
    TARGET_COMPILE_DEFINITIONS(WillEngine PRIVATE PLATFORM_WINDOWS=0)
    TARGET_COMPILE_DEFINITIONS(WillEngine PRIVATE PLATFORM_APPLE=1)
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

FIND_PACKAGE(OpenGL REQUIRED)

FIND_PACKAGE(GLM REQUIRED)
MESSAGE(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

FIND_PACKAGE(GLFW3 REQUIRED)
MESSAGE(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

FIND_PACKAGE(Assimp REQUIRED)
MESSAGE(STATUS "Found Assimp in ${ASSIMP_INCLUDE_DIR}")

# GLAD
ADD_LIBRARY(GLAD "Source/ThirdParty/Graphics/glad/src/glad.c")

SET(LIBS glfw3 opengl32 GLAD ${ASSIMP_LIBRARY})

INCLUDE_DIRECTORIES(
    "${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics/glad/include"
    "${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics/glfw-3.3.2/include"
    "${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics/glm-0.9.9.8"
    "${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics/assimp-5.0.1/include"
    "${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics"
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})

SET_TARGET_PROPERTIES(WillEngine
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_custom_command(TARGET WillEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/Source/ThirdParty/Graphics/assimp-5.0.1/build/code/Debug/assimp-vc141-mtd.dll" "E:/WillEngine/Build/Debug" )
