include_directories(${FileExchanger_SOURCE_DIR}/src/include)
include_directories(${FileExchanger_SOURCE_DIR}/tests/include)

add_library (Program Broker.cpp include/Broker.h Client.cpp include/Client.h)
add_executable (Main main.cpp)
target_link_libraries (Main Program)