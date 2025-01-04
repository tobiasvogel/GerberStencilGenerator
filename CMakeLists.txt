cmake_minimum_required(VERSION 3.16)
project(GerberStencilGenerator VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
add_definitions(-DQT_NO_KEYWORDS)
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui" "src")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Xml)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

find_package(PkgConfig REQUIRED)

qt_standard_project_setup()

qt_add_executable(GerberStencilGenerator WIN32 MACOSX_BUNDLE
    src/apertureeditview.cpp src/apertureeditview.h
    src/aperturelistwidget.cpp src/aperturelistwidget.h
    src/color.h
    src/enums.h
    src/floatcompare.h
    src/gerberplotterdialog.cpp src/gerberplotterdialog.h
    src/gerberstencilgenerator.cpp src/gerberstencilgenerator.h
    src/hollowgraphicsellipseitem.cpp src/hollowgraphicsellipseitem.h
    src/hollowgraphicsobrounditem.cpp src/hollowgraphicsobrounditem.h
    src/hollowgraphicspolygonitem.cpp src/hollowgraphicspolygonitem.h
    src/hollowgraphicsrectitem.cpp src/hollowgraphicsrectitem.h
    src/hollowroundedgraphicspolygonitem.cpp src/hollowroundedgraphicspolygonitem.h
    src/hollowroundedgraphicsrectitem.cpp src/hollowroundedgraphicsrectitem.h
    src/main.cpp
    src/renderthread.cpp src/renderthread.h
    src/settingsdialog.cpp src/settingsdialog.h
    src/shapeicon.cpp src/shapeicon.h
    src/slidersizelabel.cpp src/slidersizelabel.h
    src/snapslider.cpp src/snapslider.h
    src/tipoftheday.cpp src/tipoftheday.h
    ui/gerberplotterdialog.ui
    ui/gerberstencilgenerator.ui
    ui/settingsdialog.ui
)
target_include_directories(GerberStencilGenerator PRIVATE
    src
    ui
)

target_compile_definitions(GerberStencilGenerator PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_NO_KEYWORDS
)

target_link_libraries(GerberStencilGenerator PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Xml
)

pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(LIBGERBV REQUIRED libgerbv)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PIXMAN REQUIRED pixman-1) 
target_include_directories(GerberStencilGenerator PUBLIC
    ${GTK2_INCLUDE_DIRS}
    ${LIBGERBV_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${PIXMAN_INCLUDE_DIRS}
)
target_link_libraries(GerberStencilGenerator PUBLIC
    ${GTK2_LIBRARIES}
    ${LIBGERBV_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${PIXMAN_LIBRARIES}
)
target_compile_options(GerberStencilGenerator PUBLIC
    ${GTK2_CFLAGS_OTHER}
    ${LIBGERBV_CFLAGS_OTHER}
    ${CAIRO_CFLAGS_OTHER}
    ${PIXMAN_CFLAGS_OTHER}
)

# Resources:
set_source_files_properties("resources/apertures/circle.svg"
    PROPERTIES QT_RESOURCE_ALIAS "circle"
)
set_source_files_properties("resources/apertures/circle_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "circle-changed"
)
set_source_files_properties("resources/apertures/circle_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "circle-deleted"
)
set_source_files_properties("resources/apertures/circle_h.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-circle"
)
set_source_files_properties("resources/apertures/circle_h_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-circle-changed"
)
set_source_files_properties("resources/apertures/circle_h_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-circle-deleted"
)
set_source_files_properties("resources/apertures/macro.svg"
    PROPERTIES QT_RESOURCE_ALIAS "macro"
)
set_source_files_properties("resources/apertures/macro_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "macro-changed"
)
set_source_files_properties("resources/apertures/macro_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "macro-deleted"
)
set_source_files_properties("resources/apertures/obround.svg"
    PROPERTIES QT_RESOURCE_ALIAS "obround"
)
set_source_files_properties("resources/apertures/obround_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "obround-changed"
)
set_source_files_properties("resources/apertures/obround_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "obround-deleted"
)
set_source_files_properties("resources/apertures/obround_h.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-obround"
)
set_source_files_properties("resources/apertures/obround_h_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-obround-changed"
)
set_source_files_properties("resources/apertures/obround_h_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-obround-deleted"
)
set_source_files_properties("resources/apertures/polygon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "polygon"
)
set_source_files_properties("resources/apertures/polygon_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "polygon-changed"
)
set_source_files_properties("resources/apertures/polygon_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "polygon-deleted"
)
set_source_files_properties("resources/apertures/polygon_h.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-polygon"
)
set_source_files_properties("resources/apertures/polygon_h_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-polygon-changed"
)
set_source_files_properties("resources/apertures/polygon_h_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-polygon-deleted"
)
set_source_files_properties("resources/apertures/rectangle.svg"
    PROPERTIES QT_RESOURCE_ALIAS "rectangle"
)
set_source_files_properties("resources/apertures/rectangle_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "rectangle-changed"
)
set_source_files_properties("resources/apertures/rectangle_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "rectangle-deleted"
)
set_source_files_properties("resources/apertures/rectangle_h.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-rectangle"
)
set_source_files_properties("resources/apertures/rectangle_h_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-rectangle-changed"
)
set_source_files_properties("resources/apertures/rectangle_h_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "hollow-rectangle-deleted"
)
set_source_files_properties("resources/apertures/unknown.svg"
    PROPERTIES QT_RESOURCE_ALIAS "unknown"
)
set_source_files_properties("resources/apertures/unknown_changed.svg"
    PROPERTIES QT_RESOURCE_ALIAS "unknown-changed"
)
set_source_files_properties("resources/apertures/unknown_deleted.svg"
    PROPERTIES QT_RESOURCE_ALIAS "unknown-deleted"
)
set_source_files_properties("resources/appicon/icon.png"
    PROPERTIES QT_RESOURCE_ALIAS "appicon"
)
set_source_files_properties("resources/icons/AddBuildQueue.svg"
    PROPERTIES QT_RESOURCE_ALIAS "compile"
)
set_source_files_properties("resources/icons/Checkmark.svg"
    PROPERTIES QT_RESOURCE_ALIAS "status_ok"
)
set_source_files_properties("resources/icons/ColorPicker.svg"
    PROPERTIES QT_RESOURCE_ALIAS "color_pick"
)
set_source_files_properties("resources/icons/DynamicDiscoveryDocument.svg"
    PROPERTIES QT_RESOURCE_ALIAS "web"
)
set_source_files_properties("resources/icons/EmbedText.svg"
    PROPERTIES QT_RESOURCE_ALIAS "embedtext"
)
set_source_files_properties("resources/icons/Exit.svg"
    PROPERTIES QT_RESOURCE_ALIAS "exit"
)
set_source_files_properties("resources/icons/GenerateResource.svg"
    PROPERTIES QT_RESOURCE_ALIAS "plot"
)
set_source_files_properties("resources/icons/GlyphRight.svg"
    PROPERTIES QT_RESOURCE_ALIAS "right"
)
set_source_files_properties("resources/icons/No_Zoom.svg"
    PROPERTIES QT_RESOURCE_ALIAS "no-zoom"
)
set_source_files_properties("resources/icons/OpenFolder.svg"
    PROPERTIES QT_RESOURCE_ALIAS "open"
)
set_source_files_properties("resources/icons/Property.svg"
    PROPERTIES QT_RESOURCE_ALIAS "settings"
)
set_source_files_properties("resources/icons/Refresh.svg"
    PROPERTIES QT_RESOURCE_ALIAS "refresh"
)
set_source_files_properties("resources/icons/Save.svg"
    PROPERTIES QT_RESOURCE_ALIAS "save"
)
set_source_files_properties("resources/icons/Search.svg"
    PROPERTIES QT_RESOURCE_ALIAS "search"
)
set_source_files_properties("resources/icons/StatusCriticalError.svg"
    PROPERTIES QT_RESOURCE_ALIAS "deny"
)
set_source_files_properties("resources/icons/StatusInvalid.svg"
    PROPERTIES QT_RESOURCE_ALIAS "status_invalid"
)
set_source_files_properties("resources/icons/StatusOK.svg"
    PROPERTIES QT_RESOURCE_ALIAS "accept"
)
set_source_files_properties("resources/icons/StatusOffline_stop.svg"
    PROPERTIES QT_RESOURCE_ALIAS "status_notok"
)
set_source_files_properties("resources/icons/StatusWarning.svg"
    PROPERTIES QT_RESOURCE_ALIAS "status_warning"
)
set_source_files_properties("resources/icons/Toolbox.svg"
    PROPERTIES QT_RESOURCE_ALIAS "toolbox"
)
set_source_files_properties("resources/icons/Trash.svg"
    PROPERTIES QT_RESOURCE_ALIAS "trash"
)
set_source_files_properties("resources/icons/Untrash.svg"
    PROPERTIES QT_RESOURCE_ALIAS "untrash"
)
set_source_files_properties("resources/icons/VBWorkflowManagerActivity.svg"
    PROPERTIES QT_RESOURCE_ALIAS "recheck"
)
set_source_files_properties("resources/icons/Zoom_In.svg"
    PROPERTIES QT_RESOURCE_ALIAS "zoom-in"
)
set_source_files_properties("resources/icons/Zoom_Out.svg"
    PROPERTIES QT_RESOURCE_ALIAS "zoom-out"
)
set_source_files_properties("resources/loading.svg"
    PROPERTIES QT_RESOURCE_ALIAS "loading"
)
set_source_files_properties("resources/s_loading.svg"
    PROPERTIES QT_RESOURCE_ALIAS "s_loading"
)
set_source_files_properties("resources/splitter.svg"
    PROPERTIES QT_RESOURCE_ALIAS "splitter"
)
set(resources_resource_files
    "resources/apertures/circle.svg"
    "resources/apertures/circle_changed.svg"
    "resources/apertures/circle_deleted.svg"
    "resources/apertures/circle_h.svg"
    "resources/apertures/circle_h_changed.svg"
    "resources/apertures/circle_h_deleted.svg"
    "resources/apertures/macro.svg"
    "resources/apertures/macro_changed.svg"
    "resources/apertures/macro_deleted.svg"
    "resources/apertures/obround.svg"
    "resources/apertures/obround_changed.svg"
    "resources/apertures/obround_deleted.svg"
    "resources/apertures/obround_h.svg"
    "resources/apertures/obround_h_changed.svg"
    "resources/apertures/obround_h_deleted.svg"
    "resources/apertures/polygon.svg"
    "resources/apertures/polygon_changed.svg"
    "resources/apertures/polygon_deleted.svg"
    "resources/apertures/polygon_h.svg"
    "resources/apertures/polygon_h_changed.svg"
    "resources/apertures/polygon_h_deleted.svg"
    "resources/apertures/rectangle.svg"
    "resources/apertures/rectangle_changed.svg"
    "resources/apertures/rectangle_deleted.svg"
    "resources/apertures/rectangle_h.svg"
    "resources/apertures/rectangle_h_changed.svg"
    "resources/apertures/rectangle_h_deleted.svg"
    "resources/apertures/unknown.svg"
    "resources/apertures/unknown_changed.svg"
    "resources/apertures/unknown_deleted.svg"
    "resources/appicon/icon.png"
    "resources/icons/AddBuildQueue.svg"
    "resources/icons/Checkmark.svg"
    "resources/icons/ColorPicker.svg"
    "resources/icons/DynamicDiscoveryDocument.svg"
    "resources/icons/EmbedText.svg"
    "resources/icons/Exit.svg"
    "resources/icons/GenerateResource.svg"
    "resources/icons/GlyphRight.svg"
    "resources/icons/No_Zoom.svg"
    "resources/icons/OpenFolder.svg"
    "resources/icons/Property.svg"
    "resources/icons/Refresh.svg"
    "resources/icons/Save.svg"
    "resources/icons/Search.svg"
    "resources/icons/StatusCriticalError.svg"
    "resources/icons/StatusInvalid.svg"
    "resources/icons/StatusOK.svg"
    "resources/icons/StatusOffline_stop.svg"
    "resources/icons/StatusWarning.svg"
    "resources/icons/Toolbox.svg"
    "resources/icons/Trash.svg"
    "resources/icons/Untrash.svg"
    "resources/icons/VBWorkflowManagerActivity.svg"
    "resources/icons/Zoom_In.svg"
    "resources/icons/Zoom_Out.svg"
    "resources/loading.svg"
    "resources/s_loading.svg"
    "resources/splitter.svg"
)

qt_add_resources(GerberStencilGenerator "resources"
    PREFIX
        "/res"
    BASE
        "resources"
    FILES
        ${resources_resource_files}
)
set_source_files_properties("resources/lightbulb.svg"
    PROPERTIES QT_RESOURCE_ALIAS "lightbulb"
)
set(resources1_resource_files
    "resources/lightbulb.svg"
)

qt_add_resources(GerberStencilGenerator "resources1"
    PREFIX
        "/img"
    BASE
        "resources"
    FILES
        ${resources1_resource_files}
)
set_source_files_properties("resources/tips.json"
    PROPERTIES QT_RESOURCE_ALIAS "tips"
)
set_source_files_properties("resources/tips.schema"
    PROPERTIES QT_RESOURCE_ALIAS "json_schema"
)
set(resources2_resource_files
    "resources/tips.json"
    "resources/tips.schema"
    "resources/translations/tips_en.json"
)

qt_add_resources(GerberStencilGenerator "resources2"
    PREFIX
        "/json"
    BASE
        "resources"
    FILES
        ${resources2_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(GerberStencilGenerator PRIVATE
        Qt::Widgets
    )
endif()

if((NOT (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee)) AND (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets-master))
    target_sources(GerberStencilGenerator PUBLIC
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/abstract_widget_list.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/bound_color_selector.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_2d_slider.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_delegate.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_dialog.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_line_edit.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_list_widget.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_names.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_palette.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_palette_model.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_palette_widget.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_preview.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_selector.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_utils.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_wheel.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/color_wheel_private.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/colorwidgets_global.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/gradient_delegate.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/gradient_editor.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/gradient_helper.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/gradient_list_model.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/gradient_slider.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/harmony_color_wheel.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/hue_slider.hpp
        3rdparty/Qt-Color-Widgets-master/include/QtColorWidgets/swatch.hpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/abstract_widget_list.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/bound_color_selector.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_2d_slider.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_delegate.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_dialog.cpp 3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_dialog.ui
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_line_edit.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_list_widget.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_names.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_palette.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_palette_model.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_palette_widget.cpp 3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_palette_widget.ui
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_preview.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_selector.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_utils.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/color_wheel.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/gradient_editor.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/gradient_list_model.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/gradient_slider.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/harmony_color_wheel.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/hue_slider.cpp
        3rdparty/Qt-Color-Widgets-master/src/QtColorWidgets/swatch.cpp
    )

    target_include_directories(GerberStencilGenerator PRIVATE
        3rdparty/Qt-Color-Widgets-master/include
        3rdparty/Qt-Color-Widgets-master/src
    )


    # Resources:
    set(color_widgets_resource_files
        "3rdparty/Qt-Color-Widgets-master/resources/QtColorWidgets/alphaback.png"
    )

    qt_add_resources(GerberStencilGenerator "color_widgets"
        PREFIX
            "/color_widgets"
        BASE
            "3rdparty/Qt-Color-Widgets-master/resources/QtColorWidgets"
        FILES
            ${color_widgets_resource_files}
    )
elseif((NOT (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets-master)) AND (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee))
    target_sources(GerberStencilGenerator PUBLIC
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/abstract_widget_list.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/bound_color_selector.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_2d_slider.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_delegate.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_dialog.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_line_edit.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_list_widget.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_names.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_palette.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_palette_model.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_palette_widget.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_preview.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_selector.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_utils.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_wheel.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/color_wheel_private.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/colorwidgets_global.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/gradient_delegate.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/gradient_editor.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/gradient_helper.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/gradient_list_model.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/gradient_slider.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/harmony_color_wheel.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/hue_slider.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include/QtColorWidgets/swatch.hpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/abstract_widget_list.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/bound_color_selector.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_2d_slider.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_delegate.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_dialog.cpp 3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_dialog.ui
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_line_edit.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_list_widget.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_names.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_palette.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_palette_model.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_palette_widget.cpp 3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_palette_widget.ui
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_preview.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_selector.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_utils.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/color_wheel.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/gradient_editor.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/gradient_list_model.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/gradient_slider.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/harmony_color_wheel.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/hue_slider.cpp
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src/QtColorWidgets/swatch.cpp
    )

    target_include_directories(GerberStencilGenerator PRIVATE
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/include
        3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/src
    )


    # Resources:
    set(color_widgets_resource_files
        "3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/resources/QtColorWidgets/alphaback.png"
    )

    qt_add_resources(GerberStencilGenerator "color_widgets"
        PREFIX
            "/color_widgets"
        BASE
            "3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee/resources/QtColorWidgets"
        FILES
            ${color_widgets_resource_files}
    )
elseif((EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee) AND (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Qt-Color-Widgets-master))
    message(FATAL_ERROR "Directories \"Qt-Color-Widgets-2bc527adf371a534f6340943726b2d31627fa3ee\" and \"Qt-Color-Widgets-master\" both exist. Please remove either one.")
else()
    message(${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "Additional \"Qt-Color-Widgets\"-library required. Please read the README-file in \"3rdparty/\" directory.")
endif()

install(TARGETS GerberStencilGenerator
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET GerberStencilGenerator
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
