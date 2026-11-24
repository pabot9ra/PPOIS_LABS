cmake_minimum_required(VERSION 3.16)
project(postal_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(POSTAL_COVERAGE "Instrument production code for LLVM coverage" OFF)
if(POSTAL_COVERAGE AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "POSTAL_COVERAGE requires Clang or AppleClang")
endif()

function(configure_postal_target target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(POSTAL_COVERAGE)
        target_compile_options(${target} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    endif()
endfunction()

file(GLOB DOMAIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM DOMAIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/menu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demos.cpp")

add_library(postal_core ${DOMAIN_SOURCES})
target_include_directories(postal_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
configure_postal_target(postal_core)
if(POSTAL_COVERAGE)
    target_link_options(postal_core INTERFACE -fprofile-instr-generate -fcoverage-mapping)
endif()

add_library(postal_ui menu.cpp demos.cpp)
target_link_libraries(postal_ui PUBLIC postal_core)
configure_postal_target(postal_ui)

add_executable(postal main.cpp)
target_link_libraries(postal PRIVATE postal_ui)
configure_postal_target(postal)

include(CTest)
if(BUILD_TESTING)
    find_path(UNITTESTPP_INCLUDE_DIR UnitTest++/UnitTest++.h
        HINTS /opt/homebrew/include /usr/local/include)
    find_library(UNITTESTPP_LIBRARY NAMES UnitTest++
        HINTS /opt/homebrew/lib /usr/local/lib)
    if(NOT UNITTESTPP_INCLUDE_DIR OR NOT UNITTESTPP_LIBRARY)
        message(FATAL_ERROR "Install UnitTest++ (macOS: brew install unittest-cpp) or set UNITTESTPP_INCLUDE_DIR and UNITTESTPP_LIBRARY")
    endif()
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    file(GLOB UNIT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/*.cpp")
    add_executable(postal_tests ${UNIT_SOURCES})
    target_include_directories(postal_tests PRIVATE ${UNITTESTPP_INCLUDE_DIR})
    target_link_libraries(postal_tests PRIVATE postal_ui ${UNITTESTPP_LIBRARY})
    configure_postal_target(postal_tests)
    add_test(NAME postal_unit COMMAND postal_tests)
    add_test(NAME postal_cli COMMAND ${Python3_EXECUTABLE}
        "${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/cli_smoke.py" $<TARGET_FILE:postal>)
    if(POSTAL_COVERAGE)
        set_tests_properties(postal_unit postal_cli PROPERTIES
            ENVIRONMENT "LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/coverage-%p.profraw")
    endif()
endif()
