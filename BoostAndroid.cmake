set(BOOST_BUILD_COMPONENTS filesystem system program_options atomic thread)
unset(BUILD_WITH_LIBRARIES)
string(REPLACE ";" "," BUILD_WITH_LIBRARIES "${BOOST_BUILD_COMPONENTS}")

set(BOOST_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/boost/build)
set(BOOST_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/boost/install)
set(BOOST_EXTRACT_DIR ${CMAKE_CURRENT_BINARY_DIR}/boost)

if (FOR_ANDROID)
    set(
            BUILD_COMMAND
            ./build.sh
            --verbose
            --with-libraries=${BUILD_WITH_LIBRARIES}
            --build-dir=${BOOST_BUILD_DIR}
            --prefix=${BOOST_INSTALL_DIR}
            --extract-dir=${BOOST_EXTRACT_DIR}
            --arch=${CMAKE_ANDROID_ARCH_ABI}
            --ndk-root=$ENV{ANDROID_NDK}
    )
    set(Boost_INCLUDE_DIRS ${BOOST_INSTALL_DIR}/${CMAKE_ANDROID_ARCH_ABI}/include)
else ()
    set(
            BUILD_COMMAND
            ./build.sh
            --verbose
            --with-libraries=${BUILD_WITH_LIBRARIES}
            --build-dir=${BOOST_BUILD_DIR}
            --prefix=${BOOST_INSTALL_DIR}
            --extract-dir=${BOOST_EXTRACT_DIR}
    )
    set(Boost_INCLUDE_DIRS ${BOOST_INSTALL_DIR}/include)
endif ()

unset(COMPONENTS_LIBRARY_PATH)
unset(Boost_LIBRARIES CACHE)
foreach (COMPONENT ${BOOST_BUILD_COMPONENTS})
    if (FOR_ANDROID)
        set(COMPONENTS_LIBRARY_PATH ${COMPONENTS_LIBRARY_PATH} ${BOOST_INSTALL_DIR}/${CMAKE_ANDROID_ARCH_ABI}/lib/libboost_${COMPONENT}.a)
    else ()
        set(COMPONENTS_LIBRARY_PATH ${COMPONENTS_LIBRARY_PATH} ${BOOST_INSTALL_DIR}/lib/libboost_${COMPONENT}.a)
    endif ()
    set(Boost_LIBRARIES boost_${COMPONENT} ${Boost_LIBRARIES})
endforeach ()

add_custom_command(
        COMMENT "Building boost..."
        OUTPUT ${COMPONENTS_LIBRARY_PATH}
        COMMAND ${BUILD_COMMAND}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Boost-for-Android
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Boost-for-Android/build.sh
)

add_custom_target(build_boost ALL DEPENDS ${COMPONENTS_LIBRARY_PATH})

foreach (COMPONENT ${BOOST_BUILD_COMPONENTS})
    add_library(boost_${COMPONENT} STATIC IMPORTED GLOBAL)
    if (FOR_ANDROID)
        set(COMPONENT_LIBRARY_PATH ${BOOST_INSTALL_DIR}/${CMAKE_ANDROID_ARCH_ABI}/lib/libboost_${COMPONENT}.a)
    else ()
        set(COMPONENT_LIBRARY_PATH ${BOOST_INSTALL_DIR}/lib/libboost_${COMPONENT}.a)
    endif ()
    set_target_properties(boost_${COMPONENT} PROPERTIES IMPORTED_LOCATION ${COMPONENT_LIBRARY_PATH})
    add_dependencies(boost_${COMPONENT} build_boost)
endforeach ()

set(
        Boost_LIBRARIES
        ${Boost_LIBRARIES}
        CACHE INTERNAL "boost libraries" FORCE
)

set(
        BOOST_INC_DIR
        ${Boost_INCLUDE_DIRS}
        CACHE INTERNAL "boost include directory" FORCE
)