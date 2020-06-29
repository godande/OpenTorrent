cmake_minimum_required(VERSION 3.14)

project(CockTorrent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src/include)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread regex)

SET(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

file(GLOB SOURCES "src/*.cpp")
add_library(pthread OBJECT /usr/lib/libpthread.so)
add_executable(${PROJECT_NAME} ${SOURCES} src/include/TorrentConnectionv6.cpp src/include/TorrentConnectionv6.h src/ConnectPacket.cpp src/include/ConnectPacket.h src/ResponseConnectPacket.cpp src/include/ResponseConnectPacket.h src/AnnouncePacket.cpp src/include/AnnouncePacket.h src/ResponseAnnouncePacket.cpp src/include/ResponseAnnouncePacket.h src/TorrentConnection4.cpp src/include/TorrentConnection4.h)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

