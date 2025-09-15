
set(HEADERS
    common/worker.h
    models/IQItemListModel.h
    models/QItemListModel.h
    models/QItemSortFilterProxyModel.h
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
    README.md
    tree.txt
)


add_executable(appListModel
  ${PROJECT_SOURCES})


target_include_directories(appListModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(appListModel
                                          PUBLIC WeaNet::WeaNet
                                          PUBLIC libTools)
