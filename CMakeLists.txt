cmake_minimum_required(VERSION 3.11)

project(pepng-tempalte)

set (CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

##################
# Git Submodules #
##################

find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Adds submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodules update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMODULE_RESULT)
        if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FALT_ERROR "Unable to fetch submodules wtih ${GIT_EXECUTABLE}. Error code ${GIT_SUBMODULE_RESULT}")
        endif()
    endif()
endif() 

#########
# Cmake #
#########

if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1")
endif()

if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug;Release" FORCE)
endif()

#########
# PEPNG #
#########

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pepng)

##########
# Driver #
##########

set(EXEC main)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(${EXEC} ${SRCS})

target_link_libraries(${EXEC} pepng)

target_include_directories(${EXEC} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/pepng/include
)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    file(GLOB_RECURSE files 
        "./shaders/*"
        "./textures/*"
        "./models/*"
    )
    foreach(file ${files})
        file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} ${file})
        string(APPEND CMAKE_CXX_FLAGS " --preload-file ${file}@/${relative_file}")
        message(STATUS CMAKE_CXX_FLAGS " --preload-file ${file}@/${relative_file}")
    endforeach()
endif()