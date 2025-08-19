set(CMAKE_AUTOMOC ON)

add_library(libProperties STATIC PropertyBackground.h
    IProperties.h
    PropertyAxisRange.h)

target_link_libraries(libProperties PRIVATE libTools)
