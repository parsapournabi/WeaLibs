set(CMAKE_AUTOMOC ON)


find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Charts OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts OpenGL)


add_library(libChart STATIC
#    main.cpp
    glresources.qrc
    scenes/GLChartview.h scenes/GLChartview.cpp
    scenes/GLChartRenderer.h scenes/GLChartRenderer.cpp
    utils/GLStructures.h
    utils/GLMathUtils.h
    series/GLAbstractSeries.h
    series/GLSeriesStorage.h
    series/GLSeriesHandle.h
    series/GLEnums.h
#    MainClass.h
    series/GLSeriesItem.h
    properties/IProperties.h
    properties/PropertyAxisRange.h
    properties/PropertyBackground.h
    properties/PropertySeries.h
)

add_subdirectory(example)

target_include_directories(libChart PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/scenes ${CMAKE_CURRENT_SOURCE_DIR}/series ${CMAKE_CURRENT_SOURCE_DIR}/utils)
target_link_libraries(libChart PUBLIC libTools Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::OpenGL ${OPENGL_gl_LIBRARY} pthread OpenMP::OpenMP_CXX)
