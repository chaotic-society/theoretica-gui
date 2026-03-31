cmake_minimum_required(VERSION 3.14)
project(theoretica-gui LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Prevent Windows headers from defining min/max macros that break std::min/max
    add_compile_definitions(NOMINMAX)
    # Disable secure CRT warnings that clutter MSVC output
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

include(FetchContent)

# OpenGL
find_package(OpenGL REQUIRED)


# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.3.8"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)


# Theoretica (core)
FetchContent_Declare(
    theoretica_repo
    GIT_REPOSITORY "https://github.com/chaotic-society/theoretica.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(theoretica_repo)


# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

# ImGui library target that includes backends
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR};${imgui_SOURCE_DIR}/backends/")

# Increase ImGui precision to 32-bit
target_compile_definitions(imgui PUBLIC "ImDrawIdx=unsigned int")

# Link GLFW and OpenGL
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)


# ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(implot)

set(IMPLOT_SOURCES
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)
add_library(implot STATIC ${IMPLOT_SOURCES})
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)


# ImPlot3D
FetchContent_Declare(
    implot3d
    GIT_REPOSITORY "https://github.com/brenocq/implot3d.git"
    GIT_TAG "main"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(implot3d)

file(GLOB IMPLOT3D_SOURCES "${implot3d_SOURCE_DIR}/*.cpp")
add_library(implot3d STATIC ${IMPLOT3D_SOURCES})
target_include_directories(implot3d PUBLIC ${implot3d_SOURCE_DIR})
target_link_libraries(implot3d PUBLIC imgui implot)


# Build Theoretica GUI add-on
add_library(theoretica_gui STATIC gui.cpp)
target_include_directories(theoretica_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link all dependencies
target_link_libraries(theoretica_gui PUBLIC 
    theoretica 
    imgui 
    implot 
    implot3d
    glfw
    OpenGL::GL
)

# Build example program
add_executable(gui-example main.cpp)
target_link_libraries(gui-example PRIVATE theoretica_gui)
