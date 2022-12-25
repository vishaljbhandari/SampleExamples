# CMakeList.txt : CMake project for Basic, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Basic")
set(OutputDirectory ".sbin")
# Add source to this project's executable.
add_executable (${OutputDirectory}/Basic "Basic.cpp" "Basic.h")

# TODO: Add tests and install targets if needed.