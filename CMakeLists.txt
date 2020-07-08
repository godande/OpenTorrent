cmake_minimum_required(VERSION 3.12)

project(CockTorrent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Werror -Wextra")

set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(src/include)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.56 REQUIRED COMPONENTS system thread regex)
FIND_PACKAGE(OpenSSL REQUIRED COMPONENTS Crypto)
FIND_PACKAGE(Threads REQUIRED)
SET(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} "src/main_file/main.cpp" ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} Threads::Threads)

set_target_properties(${PROJECT_NAME} 
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" 
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


# Unit testing
file(GLOB TEST_SOURCES "src/tests/*.cpp")
add_executable(tests ${TEST_SOURCES} ${SOURCES})
target_compile_definitions(tests PUBLIC
  TEST_TORRENT_FILES_PATH=${PROJECT_SOURCE_DIR}/test-torrent-files)
target_link_libraries(tests ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
target_link_libraries(tests Threads::Threads)
set_target_properties(tests
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/lib" 
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/bin")
