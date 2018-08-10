set (CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.11.1/lib/cmake")
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})