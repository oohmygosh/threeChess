cmake_minimum_required(VERSION 3.22)
project(threeChess)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_EXE_LINKER_FLAGS "-static")
include_directories(./X/include)
link_directories(./X/lib64)
link_libraries(libeasyx.a)

add_executable(threeChess main.cpp)
target_link_libraries (threeChess -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)