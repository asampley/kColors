cmake_minimum_required ( VERSION 3.1 )
project ( kColors )
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set ( CMAKE_CXX_STANDARD 14 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin )
find_package ( OpenCV REQUIRED )
add_executable ( kColors kColors.cpp )
target_link_libraries ( kColors ${OpenCV_LIBS} )
