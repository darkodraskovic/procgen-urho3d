# Set project name
project (ProcGen)
# Define target name
set (TARGET_NAME ProcGen)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

######################################

# Set CMake minimum version and CMake policy required by UrhoCommon module
cmake_minimum_required (VERSION 3.10.2)
if (COMMAND cmake_policy)
    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
    cmake_policy (SET CMP0026 OLD)
    # Honor the visibility properties for SHARED target types only
    cmake_policy (SET CMP0063 OLD)
endif ()
# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
# Include Urho3D Cmake common module
include (UrhoCommon)

# Define source files
file (GLOB_RECURSE SRC_CPP_FILES Source/*.cpp)
file (GLOB_RECURSE SRC_H_FILES Source/*.h)
define_source_files (EXTRA_CPP_FILES ${SRC_CPP_FILES} EXTRA_H_FILES ${SRC_H_FILES})

# Setup target with resource copying
setup_main_executable ()

target_link_libraries(${TARGET_NAME} tcod)
