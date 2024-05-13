cmake_minimum_required(VERSION 3.25)

project (TestTask)

set(CMAKE_CXX_STANDARD 17)


set(SOURCES
    src/file_input_tape.cpp
    src/file_output_tape.cpp
    src/sorter.cpp
    src/tmp_file.cpp
)

set(LIBRARY_NAME engine)

add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(main app/main.cpp)
target_link_libraries(main PRIVATE ${LIBRARY_NAME})

set_target_properties(
    ${LIBRARY_NAME} main
      PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
