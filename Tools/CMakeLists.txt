add_library(libTools
    STATIC
    fps.h)

target_include_directories(libTools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libTools PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
