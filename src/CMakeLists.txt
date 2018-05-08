include_directories(${FileExchanger_SOURCE_DIR}/src/include)
include_directories(${FileExchanger_SOURCE_DIR}/tests/include)

include_directories(${FileExchanger_SOURCE_DIR}/src/modules/client/include)
add_library(Modules modules/client/src/ClientMenu.cpp modules/client/include/ClientMenu.h modules/common/src/FileTransfer.cpp modules/common/include/FileTransfer.h)

add_library (Program Broker.cpp include/Broker.h Client.cpp include/Client.h SocketWrapper.cpp include/SocketWrapper.h include/inc.h)

add_executable (Main main.cpp)
target_link_libraries (Main Program Modules pthread)

add_executable(MainBroker MainBroker.cpp)
target_link_libraries(MainBroker Program Modules pthread)

add_executable(MainClient MainClient.cpp)
target_link_libraries(MainClient Program Modules pthread)
