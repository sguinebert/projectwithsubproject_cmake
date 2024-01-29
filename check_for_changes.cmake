set(HASH_FILE "${CMAKE_BINARY_DIR}/hash_file")


file(GLOB_RECURSE WEBASSEMBLY_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/webassembly/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/webassembly/*.h"
     # Add other file extensions as needed
)

#message(STATUS "source files: ${WEBASSEMBLY_SOURCES}")
# Compute hash for each file and concatenate
set(COMBINED_HASH "")
foreach(SOURCE_FILE IN LISTS WEBASSEMBLY_SOURCES)
    file(SHA256 ${SOURCE_FILE} FILE_HASH)
    string(APPEND COMBINED_HASH ${FILE_HASH})
endforeach()

# Check if the hash has changed
if(EXISTS ${HASH_FILE})
    file(READ ${HASH_FILE} LAST_HASH)
    if(NOT LAST_HASH STREQUAL COMBINED_HASH)
        set(HASH_CHANGED TRUE)
        message(STATUS "Source changed, updating...")
        file(WRITE ${HASH_FILE} ${COMBINED_HASH})
        execute_process(COMMAND ${CMAKE_COMMAND} -E exit 0)
    endif()
else()
    set(HASH_CHANGED TRUE)
    message(STATUS "never executed, updating...")
    file(WRITE ${HASH_FILE} ${COMBINED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E exit 0)
endif()

# Update the hash file
if(NOT HASH_CHANGED)
    message(STATUS "No source changes detected.")
    execute_process(COMMAND ${CMAKE_COMMAND} -E exit 1)    # Add custom commands or steps to build the external project
    # ...
endif()
