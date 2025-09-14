set(CMAKE_AUTOMOC ON)


find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Charts OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts OpenGL)

add_executable(appExampleChart
    main.cpp

    ../glresources.qrc
    resources.qrc
    datasource.h datasource.cpp
)

target_include_directories(appExampleChart PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(appExampleChart PUBLIC libChart libTools Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::OpenGL ${OPENGL_gl_LIBRARY} pthread OpenMP::OpenMP_CXX)
