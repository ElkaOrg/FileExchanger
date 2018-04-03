include_directories(${FileExchanger_SOURCE_DIR}/src/include)
include_directories(${FileExchanger_SOURCE_DIR}/tests/include)

add_library (Program Broker.cpp include/Broker.h Client.cpp include/Client.h SocketWrapper.cpp include/SocketWrapper.h include/inc.h)
add_executable (Main main.cpp)
target_link_libraries (Main Program pthread)

add_executable(MainBroker MainBroker.cpp)
target_link_libraries(MainBroker Program pthread)