cmake_minimum_required(VERSION 3.21.2)
project( EyeSeeyou LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#required libs
find_package( OpenCV REQUIRED )
find_package( SDL2 REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} )
add_executable( EyeSeeYou src/main.cpp )
target_link_libraries( EyeSeeYou ${OpenCV_LIBS} ${SDL2_LIBRARIES} )