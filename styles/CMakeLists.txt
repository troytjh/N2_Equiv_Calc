set(STYLE_SRCS
        custom_colors.cpp
        custom_colors.h
        custom_metrics.h
        custom_style.cpp
        custom_style.h
        custom_style_p.h
        custom_stylesheets.cpp
        custom_stylesheets.h
)

add_library(Styles STATIC ${STYLE_SRCS})

target_link_libraries(Styles PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Styles PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

install(FILES ${STYLE_SRCS} DESTINATION include)
target_include_directories(Styles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
