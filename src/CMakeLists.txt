cmake_minimum_required(VERSION  3.0)
set(NAME wavToMp3)
project(${NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

#find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
#if (CMAKE_CXX_CPPCHECK)
#message("cpp check enabled")
#list(APPEND CMAKE_CXX_CPPCHECK 
#    "--enable=warning"
#    "--inconclusive"
#    "--force" 
#    "--inline-suppr"
#    "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
#)
#endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../lame/lame-3.100/include
)

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
)

add_executable (${NAME} ${SRCS})

target_link_libraries(${NAME}
    libmp3lame.a
    pthread
    m
)