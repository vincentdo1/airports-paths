cmake_minimum_required(VERSION 3.21)
project(airport_routing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Warning and sanitizer flags are GCC/Clang spelling; guard them so an MSVC build
# (the promised Windows compatibility) isn't broken by unknown flags.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra)
endif()

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_SANITIZERS)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
  else()
    message(WARNING "ENABLE_SANITIZERS is only supported for GCC/Clang; ignoring")
  endif()
endif()

find_package(Threads REQUIRED)

# ── Graph + algorithms, shared by the server, the offline tool, and the tests ──
add_library(airport_core STATIC AdjList.cpp Algorithms.cpp)

# ── Bounded worker pool ──
add_library(threadpool STATIC ThreadPool.cpp)
target_link_libraries(threadpool PUBLIC Threads::Threads)

# ── HTTP routing service ──
add_executable(server server.cpp)
target_link_libraries(server PRIVATE airport_core threadpool)
if(WIN32)
  target_link_libraries(server PRIVATE ws2_32)
endif()

# ── Offline betweenness-centrality generator ──
add_executable(main main.cpp)
target_link_libraries(main PRIVATE airport_core)

# ── Tests (Catch2, vendored single header) ──
enable_testing()

add_library(catch_main STATIC catch/catchmain.cpp)
target_include_directories(catch_main PUBLIC catch)

# The Catch suites load data files by relative path, so run them from the repo
# root. 'test' is a reserved CMake target name, hence test_adjlist for that one.
function(add_catch_test target source)
  add_executable(${target} tests/${source}.cpp)
  target_link_libraries(${target} PRIVATE catch_main ${ARGN})
  add_test(NAME ${target} COMMAND ${target} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endfunction()

add_catch_test(test_adjlist test        airport_core)
add_catch_test(test_alg     test_alg    airport_core)
add_catch_test(testRouting  testRouting airport_core)
add_catch_test(testPool     testPool    threadpool)

add_catch_test(testBFS      testBFS     airport_core)

# Integration smoke test: starts the server and checks the endpoints end to end,
# including the request-hardening cases. Needs a POSIX shell + curl, so register it
# only on Unix-like systems (including the Linux CI runner).
if(UNIX)
  add_test(
    NAME api_integration
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/api_test.sh $<TARGET_FILE:server>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endif()
