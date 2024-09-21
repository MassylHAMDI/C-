file(GLOB DATA_FILES "${CMAKE_BINARY_DIR}/data/*")
foreach(FILE ${DATA_FILES})
    message(STATUS "${FILE}")
endforeach()