# check_for_changes.cmake
# Usage: cmake -P check_for_changes.cmake <source-dir> <timestamp-file>

# Arguments
set(SOURCE_DIR "${ARGV0}")
set(TIMESTAMP_FILE "${ARGV1}")

# Find all source files
file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*")

# Check if any source files are newer than the timestamp file
foreach(SOURCE_FILE IN LISTS SOURCE_FILES)
    if (${SOURCE_FILE} IS_NEWER_THAN ${TIMESTAMP_FILE})
        message("Detected changes in ${SOURCE_FILE}. Triggering rebuild...")
        execute_process(COMMAND ${CMAKE_COMMAND} --build . --target webassembly)
        break()
    endif()
endforeach()

