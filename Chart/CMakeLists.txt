find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts)


add_executable(appChart
    main.cpp

)

target_include_directories(appChart PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(appChart PUBLIC libTools Qt${QT_VERSION_MAJOR}::Charts)
