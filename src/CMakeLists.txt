#add_library(Fuzzylib
#			FuzzyOpening.cpp ZoneExtractor.cpp Kmean.cpp GraphZone.cpp
#			)

add_library(RSILIB
			ZoneCompared.cpp GraphZoneRI.cpp ZoneRI.cpp
			)


INSTALL(TARGETS RSILIB DESTINATION lib/RSI)


# add_executable(thermal_segmentation thermal_segmentation.cpp)
# target_link_libraries(thermal_segmentation ${OpenCV_LIBS} ${vodigrex_LIBRARIES} RSILIB -lboost_unit_test_framework -lboost_filesystem -lboost_system hungarian_lib)

