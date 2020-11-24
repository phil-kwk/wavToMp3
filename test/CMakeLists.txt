cmake_minimum_required(VERSION  3.0)
set(NAME wavToMp3UnitTests)
project(${NAME} CXX)

enable_testing()

#during cygwin compilation strict c++11 leads to problems with gtest port
if(NOT ${CYGWIN})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

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
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/SampleConverter.cpp
    ReadFileTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ReadFile.cpp
    WriteFileTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/WriteFile.cpp
    FileEncodingTaskTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/FileEncodingTask.cpp
    ArgumentParserTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ArgumentParser.cpp
)

set(LIBS 
    libgtest.a 
    libgtest_main.a
    libmp3lame.a
    pthread
    m
)

target_link_libraries(${NAME} ${LIBS})