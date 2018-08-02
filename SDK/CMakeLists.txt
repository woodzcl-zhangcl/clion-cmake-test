file(GLOB_RECURSE SDK_SOURCE_FILES "*.cpp" "*.c")

set(
        SDK_SOURCE_FILES
        ${SDK_SOURCE_FILES}
        CACHE INTERNAL "sdk source files" FORCE
)

set(
        SDK_INC_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
        CACHE INTERNAL "sdk include directories"
)
