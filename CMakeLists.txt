cmake_minimum_required(VERSION 3.25)

project (TestTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code.
                        Tests in tests/*.cpp will still be enabled." ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(CTest)
include(Doctest)
include(Yaml)

set(SOURCES
    src/file_tape.cpp
    src/sorter.cpp
    src/tmp_file.cpp
    src/config.cpp
)

set(TESTFILES
    tests/main.cpp
)

set(LIBRARY_NAME engine)

add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(main app/main.cpp)
target_link_libraries(${LIBRARY_NAME} PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(main PRIVATE ${LIBRARY_NAME})
target_link_libraries(${LIBRARY_NAME} PUBLIC doctest)

set_target_properties(
    ${LIBRARY_NAME} main
      PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_subdirectory(test)
