cmake_minimum_required(VERSION 3.22.1)

project("agilV8")
set(V8_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/arm64-v8a)

include_directories(
        src/main/cpp
        src/main/cpp/base
        src/main/cpp/v8
        src/main/cpp/app
        src/main/cpp/webgl
        src/main/cpp/browser
)

aux_source_directory(src/main/cpp srcs)
aux_source_directory(src/main/cpp/base srcs)
aux_source_directory(src/main/cpp/v8 srcs)
aux_source_directory(src/main/cpp/app srcs)
aux_source_directory(src/main/cpp/webgl srcs)
aux_source_directory(src/main/cpp/browser srcs)

add_library(agilV8 SHARED ${srcs})

find_library(log-lib log)
find_library(z-lib z)
find_library(android-lib android)

##v8-start
add_library(v8_libbase SHARED IMPORTED)
set_target_properties(v8_libbase
        PROPERTIES IMPORTED_LOCATION
        ${V8_LIBS}/libv8_libbase.cr.so)
add_library(v8_libplatform SHARED IMPORTED)
set_target_properties(v8_libplatform
        PROPERTIES IMPORTED_LOCATION
        ${V8_LIBS}/libv8_libplatform.cr.so)
add_library(v8 SHARED IMPORTED)
set_target_properties(v8
        PROPERTIES IMPORTED_LOCATION
        ${V8_LIBS}/libv8.cr.so)
add_library(icui18n SHARED IMPORTED)
set_target_properties(icui18n
        PROPERTIES IMPORTED_LOCATION
        ${V8_LIBS}/libicui18n.cr.so)
add_library(icuuc SHARED IMPORTED)
set_target_properties(icuuc
        PROPERTIES IMPORTED_LOCATION
        ${V8_LIBS}/libicuuc.cr.so)
add_library(chrome_zlib SHARED IMPORTED)
set_target_properties(chrome_zlib
        PROPERTIES IMPORTED_LOCATION
        ${V8_LIBS}/libchrome_zlib.cr.so)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/include)
##v8-end

target_link_libraries(agilV8
        android EGL GLESv3 jnigraphics
        ${log-lib} ${z-lib} ${android-lib}
        v8_libbase v8_libplatform icui18n icuuc chrome_zlib v8
        )