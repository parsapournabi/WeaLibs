#cmake_minimum_required(VERSION 3.16)

#project(appRadar VERSION 0.1 LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

##find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Quick)
#find_package(Qt5 5.15 COMPONENTS Quick Gui REQUIRED)


#qt_add_executable(appappRadar
#    main.cpp
#)

#qt_add_qml_module(appappRadar
#    URI appRadar
#    VERSION 1.0
#    QML_FILES
#        Main.qml
#        SOURCES Target.h
#        SOURCES TargetModel.h
#        SOURCES TargetModel.cpp
#        QML_FILES TargetItem.qml
#        QML_FILES appRadarView.qml
#        SOURCES worker.h worker.cpp
#)

## Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
## If you are developing for iOS or macOS you should consider setting an
## explicit, fixed bundle identifier manually though.
#set_target_properties(appappRadar PROPERTIES
##    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appappRadar
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#    MACOSX_BUNDLE TRUE
#    WIN32_EXECUTABLE TRUE
#)

#target_link_libraries(appappRadar
#    PRIVATE Qt6::Quick
#    PRIVATE Qt${QT_MAJOR_VERSION} Qt::Quick
#)

#include(GNUInstallDirs)
#install(TARGETS appappRadar
#    BUNDLE DESTINATION .
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)

cmake_minimum_required(VERSION 3.14)

project(Radar  VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
set(HEADERS
    common/fps.h
    common/worker.h
    models/QItemListModel.h
    models/QItemBase.h
    models/examples/SortFilterProxy.h
    models/examples/TargetModel.h
    models/examples/Target.h
)
set(SOURCES
  main.cpp
  common/worker.cpp

)
set(QML_FILES
        resources/qml.qrc
)
set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
    ${QML_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(appRadar
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET appRadar APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(appRadar SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(appRadar
          ${PROJECT_SOURCES}

        )
    endif()
endif()

target_link_libraries(appRadar
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

target_include_directories(appRadar PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appRadar)
endif()
set_target_properties(appRadar PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS appRadar
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(appRadar)
    qt_finalize_executable(appRadar)
endif()
