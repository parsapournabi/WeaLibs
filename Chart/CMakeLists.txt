cmake_minimum_required(VERSION 3.14)
project(WeaChart VERSION 1.5 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMKAE_INCLUDE_CURRENT_DIR TRUE)

find_package(OpenGL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Quick Qml OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick Qml OpenGL)

# Generating config.h form config.h.in (WeaChart_QML_IMPORT_PATH) must be assign from CMake.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/include/WeaChart/config.h)
set(WeaChart_QML_IMPORT_PATH "${CMAKE_INSTALL_PREFIX}/share/qml")


add_library(WeaChart SHARED
    tree.txt
    install.sh
    uninstall.sh
    README.md
    docs/README.md
    WeaChart.qdocconf
    cmake/WeaChartConfig.cmake.in
    cmake/WeaChartUninstall.cmake.in
    cmake/config.h.in
    docs/document.html

    resources.qrc

    include/WeaChart/GLChart
    include/WeaChart/scenes/GLChartview.h src/scenes/GLChartview.cpp
    include/WeaChart/scenes/GLChartRenderer.h src/scenes/GLChartRenderer.cpp
    include/WeaChart/utils/GLStructures.h
    include/WeaChart/utils/GLMathUtils.h
    include/WeaChart/series/GLAbstractSeries.h
    include/WeaChart/series/GLSeriesStorage.h
    include/WeaChart/series/GLSeriesHandle.h
    include/WeaChart/series/GLEnums.h
    include/WeaChart/series/GLSeriesItem.h
    include/WeaChart/properties/IProperties.h
    include/WeaChart/properties/PropertyAxisRange.h
    include/WeaChart/properties/PropertyBackground.h
    include/WeaChart/properties/PropertySeries.h
    include/WeaChart/WeaChart_export.h
    include/WeaChart/overlay/tools/GLChartItemBase.h include/WeaChart/overlay/tools/GLChartItemBase.cpp
    include/WeaChart/overlay/tools/GLChartLineItem.h include/WeaChart/overlay/tools/GLChartLineItem.cpp
    include/WeaChart/overlay/tools/GLChartGateItem.h include/WeaChart/overlay/tools/GLChartGateItem.cpp
    include/WeaChart/overlay/tools/GLChartVerticalLineItem.h include/WeaChart/overlay/tools/GLChartVerticalLineItem.cpp
    include/WeaChart/overlay/tools/GLChartHorizontalLineItem.h include/WeaChart/overlay/tools/GLChartHorizontalLineItem.cpp
    include/WeaChart/overlay/tools/GLChartVerticalGateItem.h include/WeaChart/overlay/tools/GLChartVerticalGateItem.cpp
    include/WeaChart/overlay/tools/GLChartHorizontalGateItem.h include/WeaChart/overlay/tools/GLChartHorizontalGateItem.cpp
)

# Referer include/WeaChart/WeaChart_export.h
target_compile_definitions(WeaChart PRIVATE WeaChart_EXPORT)

target_include_directories(WeaChart PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(WeaChart PUBLIC Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::OpenGL ${OPENGL_gl_LIBRARY})

# Library Install (WeaChart)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/WeaChartConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WeaChartConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/WeaChartConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WeaChart
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WeaChartUninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/WeaChartUninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/WeaChartUninstall.cmake
)

# Specifying Installation paths.
install(TARGETS WeaChart
    EXPORT WeaChartTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/WeaChartConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/WeaChartConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WeaChart
)
install(EXPORT WeaChartTargets
    FILE WeaChartTargets.cmake
    NAMESPACE WeaChart::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WeaChart)

# Copying all .qml files into $INSTALL_PREFIX/share/qml/com/wearily/WeaChart
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qml/
    DESTINATION ${WeaChart_QML_IMPORT_PATH}/com/wearily/WeaChart
    FILES_MATCHING PATTERN "*.qml" PATTERN "qmldir"
)
# Copying plugins.qmltypes file into $INSTALL_PREFIX/share/qml/com/wearily/WeaChart
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qml/plugins.qmltypes
    DESTINATION ${WeaChart_QML_IMPORT_PATH}/com/wearily/WeaChart/
)

# Removing Duplicate WeaChart directories.
install(DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR}/WeaChart DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# Sub Directories
option(BUILD_EXAMPLE "Build example" ON)
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()


#message(STATUS
#    "CMakeAttributes: "
#    "\nCMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}"
#    "\nCMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}"
#    "\nCMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}"
#    "\nCMAKE_INSTALL_INCLUDEDIR: ${CMAKE_INSTALL_INCLUDEDIR}"
#    "\nCMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}"
#    "\nCMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}"
#    "\nPROJECT_VERSION: ${PROJECT_VERSION}"
#)
