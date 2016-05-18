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

include_directories(include ${vodigrex_INCLUDE_DIRS} )

add_subdirectory(src)
add_subdirectory(include)

# add_executable(rsi main.cpp)
# target_link_libraries(rsi ${OpenCV_LIBS})

add_executable(test_function_fuzzy Test/test_function_fuzzy.cpp)
target_link_libraries(test_function_fuzzy ${OpenCV_LIBS} Fuzzylib -lboost_unit_test_framework)

add_executable(test_watershed Test/test_watershed.cpp)
target_link_libraries(test_watershed ${OpenCV_LIBS} Fuzzylib -lboost_unit_test_framework)

add_executable(test_kmeans Test/test_kmeans.cpp)
target_link_libraries(test_kmeans ${OpenCV_LIBS} Fuzzylib -lboost_unit_test_framework)

add_executable(test_sketchify Test/test_sketchify.cpp)
target_link_libraries(test_sketchify ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)

# install(TARGETS rsi RUNTIME DESTINATION bin)

add_subdirectory(Test)
