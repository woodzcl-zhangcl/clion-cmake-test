file(GLOB_RECURSE WEBRTC_COMMON_SOURCE_FILES "*.c" "*.cc")

set(
        WEBRTC_COMMON_SOURCE_FILES
        ${WEBRTC_COMMON_SOURCE_FILES}
        CACHE INTERNAL "WEBRTC_Common source files" FORCE
)

set(
        WEBRTC_COMMON_INC_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        CACHE INTERNAL "WEBRTC_Common include directories"
)