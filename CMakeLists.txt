cmake_minimum_required(VERSION 3.16)

project(Geofence_Map VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Core SerialPort)
find_package(Qt6 COMPONENTS WebEngineQuick REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS WebEngineQuick)

qt_standard_project_setup()

qt_add_executable(appGeofence_Map
    main.cpp serial.h serial.cpp netcheck.h runnable_net.h nmea.h
)

qt_add_qml_module(appGeofence_Map
    URI Geofence_Map
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES geofence.h geofence.cpp
        SOURCES gcp.h gcp.cpp
    )
    # Resources:
    set(resources_resource_files
        "mapbuild.html"
        "reload.png"
        "navigation.png"
    )
    qt_add_resources(appGeofence_Map "resources"
        PREFIX
            "/"
        FILES
            ${resources_resource_files}
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGeofence_Map PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGeofence_Map
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGeofence_Map
    PRIVATE Qt6::Quick Qt6::SerialPort
    Qt6::Core
    Qt6::WebEngineQuick
)

include(GNUInstallDirs)
install(TARGETS appGeofence_Map
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
