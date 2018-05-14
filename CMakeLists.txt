cmake_minimum_required(VERSION 3.9)
project(FileExchanger)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

add_test (NAME MyTest COMMAND Test) #???
