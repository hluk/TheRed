project(thered)

cmake_minimum_required(VERSION 2.8)

list(APPEND thered_SOURCES main.c darken.c)
file(GLOB thered_ICONS images/thered_*.svg)

# If GTK 3 not found try to use GTK 2.
find_package(PkgConfig)
if (PkgConfig_FOUND)
    pkg_check_modules(GTK3 gtk+-3.0)
endif()

if (GTK3_FOUND)
    message(STATUS "Building with GTK 3")
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})
    list(APPEND thered_LIBRARIES ${GTK3_LIBRARIES})
else()
    find_package(GTK2)

    if (GTK2_FOUND)
        include_directories(${GTK2_INCLUDE_DIRS})
        list(APPEND thered_LIBRARIES ${GTK2_LIBRARIES})
    else()
        pkg_check_modules(GTK2 gtk+-2.0)
        include_directories(${GTK2_INCLUDE_DIRS})
        link_directories(${GTK2_LIBRARY_DIRS})
        add_definitions(${GTK2_CFLAGS_OTHER})
        list(APPEND thered_LIBRARIES ${GTK2_LIBRARIES})
    endif()

    if (GTK2_FOUND)
        message(STATUS "Building with GTK ${GTK2_VERSION}")
    else()
        message(FATAL_ERROR "Missing Gtk library!")
    endif()
endif()

# platform-dependent files
if (WIN32)
    # TODO: Add application icon.

    list(APPEND thered_SOURCES darken_win.c)

    list(APPEND thered_LIBRARIES -lgdi32)

    set(thered_ICON_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/images")

    add_definitions( -DINSTALL_PREFIX_IMAGES=images )

    add_executable(thered WIN32 ${thered_SOURCES})

    set_target_properties(thered PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
else()
    # If window system is X11 then set X11_FOUND to TRUE.
    find_package(X11 REQUIRED)

    list(APPEND thered_SOURCES darken_x11.c libkeybinder/bind.c)

    list(APPEND thered_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})

    set(thered_ICON_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps")

    add_definitions( -DHAS_SHORTCUTS -DINSTALL_PREFIX_IMAGES=${thered_ICON_INSTALL_PREFIX} )

    add_executable(thered ${thered_SOURCES})
endif()

target_link_libraries(thered ${thered_LIBRARIES})

install(FILES ${thered_ICONS} DESTINATION ${thered_ICON_INSTALL_PREFIX})

install(TARGETS thered DESTINATION bin)

