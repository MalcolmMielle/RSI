add_library(Fuzzylib
			FuzzyOpening.cpp ZoneExtractor.cpp Kmean.cpp GraphZone.cpp
			)

add_executable(segmentation_example segmentation_example.cpp)
target_link_libraries(segmentation_example ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework -lboost_filesystem -lboost_system hungarian_lib)

add_executable(thermal_segmentation thermal_segmentation.cpp)
target_link_libraries(thermal_segmentation ${OpenCV_LIBS} ${vodigrex_LIBRARIES} Fuzzylib -lboost_unit_test_framework -lboost_filesystem -lboost_system hungarian_lib)
