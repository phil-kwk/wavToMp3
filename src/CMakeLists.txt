cmake_minimum_required(VERSION  3.0)
set(NAME wavToMp3)
project(${NAME} CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

#lame include_directories may need adjustment depending on where your lame include is
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../lame/lame-3.100/include
)
#lame link_directories may need adjustment depending on where your libmp3lame libs are 
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../lame/lame-3.100/libmp3lame/.libs
)

set(SRCS
    main.cpp
    Parser.cpp
    Encoder.cpp
    LameEncoder.cpp
    FileSystem.cpp
    Converter.cpp
    ReadFile.cpp
    WriteFile.cpp
    FileEncodingTask.cpp
    ArgumentParser.cpp
)

set(LIBS
    libmp3lame.a
    pthread
    m
)

add_executable (${NAME} ${SRCS})

target_link_libraries(${NAME} ${LIBS})