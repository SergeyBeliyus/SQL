cmake_minimum_required(VERSION 3.20)
project(SQLProject)
add_executable(SQLProject main.cpp SQLWork.cpp SQLWork.h)
add_subdirectory(.//libpqxx libpqxx-build)
target_compile_features(SQLProject PRIVATE cxx_std_17)
target_link_libraries(SQLProject pqxx)