# by MRB 
set(BUILD_DIR "${CMAKE_BINARY_DIR}/bin")

file(GLOB_RECURSE SE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

if(NOT SE_SOURCES)
    message(FATAL_ERROR "No source files found in ${SRC_DIR}")
endif()

add_executable(APP ${SE_SOURCES})

set_target_properties(APP PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
)
