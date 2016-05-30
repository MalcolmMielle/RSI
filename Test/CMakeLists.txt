add_executable(test_function_fuzzy test_function_fuzzy.cpp)
target_link_libraries(test_function_fuzzy ${OpenCV_LIBS} Fuzzylib -lboost_unit_test_framework)

add_executable(test_watershed test_watershed.cpp)
target_link_libraries(test_watershed ${OpenCV_LIBS} Fuzzylib -lboost_unit_test_framework)

add_executable(test_kmeans test_kmeans.cpp)
target_link_libraries(test_kmeans ${OpenCV_LIBS} Fuzzylib -lboost_unit_test_framework)

add_executable(test_sketchify test_sketchify.cpp)
target_link_libraries(test_sketchify ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)

add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)

add_executable(test_histogram test_histogram.cpp)
target_link_libraries(test_histogram ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)

add_executable(test_zonegraph test_zonegraph.cpp)
target_link_libraries(test_zonegraph ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)

add_executable(test_edit test_edit.cpp)
target_link_libraries(test_edit ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)

add_executable(test_zonereduce test_zonereduce.cpp)
target_link_libraries(test_zonereduce ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework)