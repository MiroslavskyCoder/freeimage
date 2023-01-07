cmake_minimum_required(VERSION 3.0.0)

include(GNUInstallDirs)

project(FreeImage C CXX)

if(MSVC)
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_CXX_FLAGS "/wd4828 ${CMAKE_CXX_FLAGS}")
endif()

find_package(ZLIB           REQUIRED)
find_package(PNG            REQUIRED)
find_package(JPEG           REQUIRED)
find_package(TIFF           REQUIRED)
find_package(OpenJPEG       REQUIRED)
find_package(WebP CONFIG    REQUIRED)
find_package(JXR            REQUIRED)
find_package(LibRaw         REQUIRED)
find_package(OpenEXR        REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib) 

IF(MSVC)
    OPTION(INSTALL_PDB "Also install .pdb files" ON)
ELSE()
    SET(INSTALL_PDB OFF)
ENDIF()

set(CMAKE_DEBUG_POSTFIX ".Debug")

set(REAL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(REAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(FREEIMAGE_HEADERS 
    ${REAL_INCLUDE_DIR}/FreeImage.h
)

set(ROOT_PRIVATE_HEADERS 
    ${REAL_INCLUDE_DIR}/CacheFile.h
    ${REAL_INCLUDE_DIR}/FreeImageIO.h
    ${REAL_INCLUDE_DIR}/MapIntrospector.h
    ${REAL_INCLUDE_DIR}/Plugin.h
    ${REAL_INCLUDE_DIR}/Quantizers.h
    ${REAL_INCLUDE_DIR}/ToneMapping.h
    ${REAL_INCLUDE_DIR}/Utilities.h
)

file(GLOB FREEIMAGE_PRIVATE_HEADERS ${REAL_SOURCE_DIR}/FreeImage/*.h)
file(GLOB FREEIMAGE_TOOLKIT_PRIVATE_HEADERS ${REAL_SOURCE_DIR}/FreeImageToolkit/*.h)
file(GLOB METADATA_PRIVATE_HEADERS ${REAL_SOURCE_DIR}/Metadata/*.h)

file(GLOB DEPRECATION_SRCS ${REAL_SOURCE_DIR}/DeprecationManager/*.cpp)
file(GLOB FREEIMAGE_TOOLKIT_SRCS ${REAL_SOURCE_DIR}/FreeImageToolkit/*.cpp)
file(GLOB FREEIMAGE_SRCS ${REAL_SOURCE_DIR}/FreeImage/*.cpp)
file(GLOB METADATA_SRCS ${REAL_SOURCE_DIR}/Metadata/*.cpp)

list(REMOVE_ITEM FREEIMAGE_SRCS ${REAL_SOURCE_DIR}/FreeImage/PluginG3.cpp)
list(REMOVE_ITEM FREEIMAGE_TOOLKIT_SRCS ${REAL_SOURCE_DIR}/FreeImageToolkit/JPEGTransform.cpp)


set(SRCS 
    ${DEPRECATION_SRCS}
    ${FREEIMAGE_SRCS}
    ${FREEIMAGE_TOOLKIT_SRCS}
    ${METADATA_SRCS}
)

if(BUILD_SHARED_LIBS)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/FreeImageConfig-dynamic.h ${CMAKE_CURRENT_BINARY_DIR}/FreeImageConfig.h)
else()
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/FreeImageConfig-static.h ${CMAKE_CURRENT_BINARY_DIR}/FreeImageConfig.h)
endif()

add_library(FreeImage ${SRCS} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/FreeImage.rc)

target_include_directories(FreeImage PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${ZLIB_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIRS}
    ${TIFF_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENJPEG_INCLUDE_DIRS}
    ${JXR_INCLUDE_DIRS}
    ${LibRaw_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(FreeImage 
    ${ZLIB_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${TIFF_LIBRARIES}
    ${PNG_LIBRARIES}
    ${OPENJPEG_LIBRARIES}
    WebP::webp WebP::webpdemux WebP::libwebpmux WebP::webpdecoder
    ${JXR_LIBRARIES}
    ${LibRaw_LIBRARIES}
    OpenEXR::OpenEXR
    Imath::Imath
)

target_compile_definitions(FreeImage PRIVATE ${PNG_DEFINITIONS})
 
file(GLOB FREEIMAGEPLUS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/FreeImagePlus/src/*.cpp)

add_library(FreeImagePlus
    ${FREEIMAGEPLUS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeImagePlus/FreeImagePlus.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeImagePlus/FreeImagePlus.rc
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(FreeImagePlus PRIVATE -DFIP_EXPORTS)
else()
    target_compile_definitions(FreeImagePlus PRIVATE -DFREEIMAGE_LIB)
endif()

target_include_directories(FreeImagePlus PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeImagePlus
    ${CMAKE_CURRENT_BINARY_DIR}
    ${REAL_SOURCE_DIR}
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(FreeImagePlus PUBLIC FreeImage)

list(APPEND PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/FreeImagePlus/FreeImagePlus.h)
