cmake_minimum_required(VERSION 2.6)
project(rsi)

find_package( OpenCV REQUIRED)
find_package( bettergraph REQUIRED)
find_package( vodigrex REQUIRED)
find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()

add_definitions(-std=c++11)

include_directories(include ${vodigrex_INCLUDE_DIRS} )

add_subdirectory(src)
add_subdirectory(include)

# add_executable(rsi main.cpp)
# target_link_libraries(rsi ${OpenCV_LIBS})



# install(TARGETS rsi RUNTIME DESTINATION bin)

add_subdirectory(Test)
