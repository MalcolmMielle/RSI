
get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(RSI_INCLUDE_DIRS "${SELF_DIR}/../../include/RSI;${SELF_DIR}/../../include/RSI/hungarian" ABSOLUTE)
set(RSI_LIBRARIES "${SELF_DIR}/libRSILIB.a;${SELF_DIR}/libhungarian_lib.a")
