#add_library(Fuzzylib
#			FuzzyOpening.cpp ZoneExtractor.cpp Kmean.cpp GraphZone.cpp
#			)

add_library(RSILIB
			FuzzyOpening.cpp ZoneExtractor.cpp Kmean.cpp GraphZone.cpp ZoneCompared.cpp Zone.cpp GraphZoneLight.cpp ZoneComparedLight.cpp ZoneLight.cpp
			)


INSTALL(TARGETS RSILIB DESTINATION lib/RSI)

add_executable(segmentation_example segmentation_example.cpp)
target_link_libraries(segmentation_example ${OpenCV_LIBS} ${vodigrex_LIBRARIES} RSILIB -lboost_unit_test_framework -lboost_filesystem -lboost_system)

# add_executable(evaluation_gt evaluation_gt.cpp)
# target_link_libraries(evaluation_gt ${OpenCV_LIBS} ${vodigrex_LIBRARIES} RSILIB -lboost_unit_test_framework -lboost_filesystem -lboost_system hungarian_lib)
# 
# add_executable(thermal_segmentation thermal_segmentation.cpp)
# target_link_libraries(thermal_segmentation ${OpenCV_LIBS} ${vodigrex_LIBRARIES} RSILIB -lboost_unit_test_framework -lboost_filesystem -lboost_system hungarian_lib)
