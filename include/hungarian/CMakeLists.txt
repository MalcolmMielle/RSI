
add_library(hungarian_lib
			hungarian.c
			)
			
INSTALL(TARGETS hungarian_lib DESTINATION lib/RSI)
