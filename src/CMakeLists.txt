set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

find_package(Boost 1.56.0 REQUIRED COMPONENTS system filesystem)

include_directories(${FileExchanger_SOURCE_DIR}/src/include)
include_directories(${FileExchanger_SOURCE_DIR}/tests/include)

include_directories(${FileExchanger_SOURCE_DIR}/src/modules/client/include)
add_library(Modules modules/client/src/ClientMenu.cpp modules/client/include/ClientMenu.h modules/common/src/FileTransfer.cpp modules/common/include/FileTransfer.h include/ConstVariables.h modules/client/src/ClientConnection.cpp)

add_library (Program Broker.cpp include/Broker.h SocketWrapper.cpp include/SocketWrapper.h include/inc.h include/DataBase.h include/DirManagment.h)

add_executable (Main main.cpp include/DataBase.h include/ConstVariables.h include/Message.h modules/client/include/ClientConnection.h include/FileWait.h)
target_link_libraries (Main Program Modules pthread boost_system)

add_executable(MainBroker MainBroker.cpp)
target_link_libraries(MainBroker Program Modules pthread boost_system boost_filesystem)

add_executable(MainClient MainClient.cpp)
target_link_libraries(MainClient Program Modules pthread boost_system boost_filesystem)
