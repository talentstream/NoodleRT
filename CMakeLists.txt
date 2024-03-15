cmake_minimum_required(VERSION 3.27)
project(NoodleRT)

set(CMAKE_CXX_STANDARD 23)

# 生成的dll 文件输出到与exe 同级
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
# stb
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
# glfw
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
)
# glad
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)
# pugixml
FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
)
# tinyobjloader
FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG release
)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG master
)

FetchContent_MakeAvailable(
        stb
        glfw
        glad
        pugixml
        tinyobjloader
        doctest
)


# 添加所有头文件和源文件
file(GLOB_RECURSE HEADER_FILES inc/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(NoodleRT
        ${HEADER_FILES}
        ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        inc
        ${stb_SOURCE_DIR}
        ${tinyobjloader_SOURCE_DIR}
        ${glfw_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glad
        pugixml
        #doctest
)

# tests
file(GLOB_RECURSE TEST_FILES tests/*.cpp)

add_executable(Tests
        ${TEST_FILES}
        ${HEADER_FILES}
        src/core/propertylist.cpp
)

target_include_directories(Tests PRIVATE
        inc
        ${stb_SOURCE_DIR}
        ${tinyobjloader_SOURCE_DIR}
        ${doctest_SOURCE_DIR}/doctest
)

target_link_libraries(Tests PRIVATE
        pugixml
        doctest::doctest
)

