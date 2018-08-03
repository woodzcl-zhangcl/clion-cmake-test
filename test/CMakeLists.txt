aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SOURCE_FILES)

foreach(srcFile ${TEST_SOURCE_FILES})
    string(REGEX REPLACE ".*/\(.*\).cpp$" "\\1" TEST_TARGET_NAME ${srcFile})
    add_executable(${TEST_TARGET_NAME} ${srcFile})
    target_link_libraries(${TEST_TARGET_NAME} ${Boost_LIBRARIES})
endforeach()