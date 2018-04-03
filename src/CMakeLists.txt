include_directories(${FileExchanger_SOURCE_DIR}/src/include)
include_directories(${FileExchanger_SOURCE_DIR}/tests/include)

add_library (Program Broker.cpp include/Broker.h Client.cpp include/Client.h)
add_executable (demo main.cpp)
target_link_libraries (demo Program)