cmake_minimum_required(VERSION 3.16)
project(qt6-screencopy VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core WaylandClient)
qt_standard_project_setup()

include_directories(src)

set(lib_SOURCES
    src/screencopy.cpp
    src/screencopy_p.h
    src/qt6-screencopy.h)

qt6_add_library(qt6-screencopy
    SHARED ${lib_SOURCES}
)

set_target_properties(qt6-screencopy PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(qt6-screencopy PROPERTIES SOVERSION 0)
set_target_properties(qt6-screencopy PROPERTIES PUBLIC_HEADER src/qt6-screencopy.h)

qt6_generate_wayland_protocol_client_sources(
    qt6-screencopy
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/protocols/wlr-screencopy-unstable-v1.xml
)

target_link_libraries(qt6-screencopy PUBLIC
    Qt::Core
    Qt::WaylandClient
    Qt::WaylandClientPrivate
)

configure_file(src/qt6-screencopy.pc.in qt6-screencopy.pc @ONLY)

install(TARGETS qt6-screencopy
    RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    BUNDLE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES ${CMAKE_BINARY_DIR}/qt6-screencopy.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
