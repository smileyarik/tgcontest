cmake_minimum_required(VERSION 3.0.2)

project(tgnews)

include(CTest)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Wno-sign-compare")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O2 -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops -Ofast -march=native")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

set(BUILD_TESTING 0)
add_subdirectory(thirdparty/fasttext EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/tinyxml2)
add_subdirectory(thirdparty/eigen EXCLUDE_FROM_ALL)
set(LIB_ONLY True)
add_subdirectory(thirdparty/onmt_tokenizer EXCLUDE_FROM_ALL)
unset(LIB_ONLY)
unset(BUILD_TESTING)

find_package(Boost COMPONENTS program_options filesystem unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(SOURCE_FILES
    src/agency_rating.cpp
    src/annotate.cpp
    src/cluster.cpp
    src/clustering/slink.cpp
    src/detect.cpp
    src/document.cpp
    src/embedder.cpp
    src/rank.cpp
    src/summarize.cpp
    src/thread_pool.cpp
    src/util.cpp
)

set(HEADER_FILES
    src/agency_rating.h
    src/annotate.h
    src/cluster.h
    src/clustering/clustering.h
    src/clustering/slink.h
    src/detect.h
    src/document.h
    src/embedder.h
    src/rank.h
    src/summarize.h
    src/thread_pool.h
    src/timer.h
    src/util.h
)

set(LIB_LIST
    ${Boost_LIBRARIES}
    fasttext-static
    tinyxml2
    eigen
    OpenNMTTokenizer
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fasttext/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/onmt_tokenizer/include")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_LIST})

enable_testing()

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)
add_definitions(-DTEST_PATH=${CMAKE_CURRENT_SOURCE_DIR}/test)
foreach(testSrc ${TEST_SRCS})
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${SOURCE_FILES} ${testSrc})
    target_link_libraries(${testName} PRIVATE ${LIB_LIST})
    add_test(NAME ${testName} COMMAND ${testName})
endforeach(testSrc)
