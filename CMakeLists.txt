cmake_minimum_required(VERSION 3.16)
project(Vidupe
    VERSION 1.211
    DESCRIPTION "Vidupe is a program that can find duplicate and similar video files."
    LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql Widgets)

if(APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME "${CMAKE_PROJECT_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_INFO_STRING "${CMAKE_PROJECT_NAME}: ${CMAKE_PROJECT_DESCRIPTION}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${CMAKE_PROJECT_VERSION}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}")
endif()

set(SOURCE_FILES
    src/comparison.cpp
    src/db.cpp
    src/mainwindow.cpp
    src/osutils.cpp
    src/ssim.cpp
    src/video.cpp)

set(HEADERS
    src/comparison.h
    src/db.h
    src/mainwindow.h
    src/osutils.h
    src/prefs.h
    src/thumbnail.h
    src/video.h)

set(FORMS
    src/comparison.ui
    src/mainwindow.ui)

if(WIN32 OR APPLE)
    include(cmake/AddIconToBinary.cmake)
    AddIconToBinary(SOURCE_FILES ICONS res/vidupe16.ico res/vidupe16.icns)
endif()

if(WIN32)
    add_executable(Vidupe WIN32 ${SOURCE_FILES} ${HEADERS} ${FORMS})
elseif(APPLE)
    add_executable(Vidupe MACOSX_BUNDLE ${SOURCE_FILES} ${HEADERS} ${FORMS})
else()
    add_executable(Vidupe ${SOURCE_FILES} ${HEADERS} ${FORMS})
endif()

target_compile_definitions(Vidupe PRIVATE
    APP_COPYRIGHT="Copyright \\251 2018-2019 Kristian Koskim\\344ki"
    APP_NAME="${CMAKE_PROJECT_NAME}"
    APP_VERSION="${CMAKE_PROJECT_VERSION}")

find_package(OpenCV REQUIRED core imgproc videoio)

target_include_directories(Vidupe PRIVATE src)
target_link_libraries(Vidupe PRIVATE ${OpenCV_LIBS} Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS Vidupe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

file(
    GENERATE
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/CopyExtensionsIni.cmake"
        CONTENT
            "if(NOT EXISTS \"$<TARGET_FILE_DIR:${PROJECT_NAME}>/extensions.ini\")
                execute_process(
                    COMMAND \"${CMAKE_COMMAND}\" -E copy
                        \"${PROJECT_SOURCE_DIR}/extensions.ini\"
                        \"$<TARGET_FILE_DIR:${PROJECT_NAME}>\")
            endif()")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -P "CopyExtensionsIni.cmake"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
