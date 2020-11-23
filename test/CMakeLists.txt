cmake_minimum_required(VERSION  3.0)
set(NAME testcases)
project(${NAME} CXX)

enable_testing()

#during cygwin compilation strict c++11 leads to problems with gtest port
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../gtest/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../lame/lame-3.100/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../gtest/googletest/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../lame/lame-3.100/libmp3lame/.libs
)

add_executable(${NAME}
    tests.cpp
    TestWaveData.cpp
    ParserTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Parser.cpp
    EncoderTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Encoder.cpp
    FileStructureTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/FileSystem.cpp
    LameEncodingFileTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/LameEncoder.cpp
    ConverterTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Converter.cpp
    ReadFileTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ReadFile.cpp
    WriteFileTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/WriteFile.cpp
    FileEncodingTaskTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/FileEncodingTask.cpp
)

if(${CYGWIN})
    set(LAME_LIBS libmp3lame.dll.a)
else()
    set(LAME_LIBS libmp3lame.a)
endif(${CYGWIN})

set(LIBS 
    libgtest.a 
    libgtest_main.a
    ${LAME_LIBS}
    pthread
    m
)

target_link_libraries(${NAME} ${LIBS})