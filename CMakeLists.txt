cmake_minimum_required(VERSION 3.16)

project(Schematic_Conversion_Factor VERSION 1.0 LANGUAGES CXX)

set(QT_VERSION_MAJOR 6)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -whole-archive")
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll.a")
#set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

## Add MSYS2 Packages To Search Path
#set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Users/Troy.Hoover/Programs/msys64/mingw64")

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Pdf)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Schematic_Conversion_Factor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        pdf_document.h pdf_document.cpp
        icons.qrc
        fonts.qrc
        settingswindow.h settingswindow.cpp settingswindow.ui
        conversion_factors.h conversion_factors.cpp
    )
endif()

target_link_libraries(Schematic_Conversion_Factor PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Schematic_Conversion_Factor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Schematic_Conversion_Factor PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(Schematic_Conversion_Factor PRIVATE Qt${QT_VERSION_MAJOR}::Pdf)
target_link_libraries(Schematic_Conversion_Factor PRIVATE Styles)

#find_file(libwinpthread_path libwinpthread-1.dll)
#find_file(libstdc++-6_path libstdc++-6.dll)
#find_file(libgcc_s_seh_path libgcc_s_seh-1.dll)

#install(
#    FILES
#        $<TARGET_FILE:Qt6::Core>
#        $<TARGET_FILE:Qt6::Gui>
#        $<TARGET_FILE:Qt6::Widgets>
#        $<TARGET_FILE:Qt6::Network>
#        $<TARGET_FILE:Qt6::Pdf>
#        $<TARGET_FILE:Qt6::Sql>
#        ${libwinpthread_path}
#        ${libstdc++-6_path}
#        ${libgcc_s_seh_path}
#    DESTINATION bin
#)

set_target_properties(Schematic_Conversion_Factor PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Schematic_Conversion_Factor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Deploy DLLS required at runtime
if(CMAKE_HOST_WIN32)
    set(deploy_tool_options_arg
            --release
            --no-translations
            --no-system-d3d-compiler
            --no-system-dxc-compiler
            --no-opengl-sw)

    # Run the windeployqt tool after building
    add_custom_command(
        TARGET Schematic_Conversion_Factor
        POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE}
            ${deploy_tool_options_arg}
            --dir dll_libs
            $<TARGET_FILE:Schematic_Conversion_Factor>
        COMMENT "Deploying application with windeployqt..."
    )
endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/dll_libs/ DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Schematic_Conversion_Factor)
endif()

add_subdirectory(styles)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/styles)
