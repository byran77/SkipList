cmake_minimum_required(VERSION 3.5)

project(ranklist)

add_executable(ranklist
    ${PROJECT_SOURCE_DIR}/example/rankListTest.cpp
)

target_include_directories(ranklist
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/detail
        ${PROJECT_SOURCE_DIR}/example/include
        ${PROJECT_SOURCE_DIR}/example/detail
)