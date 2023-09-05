cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME traact_component_cereal)

project(${TARGET_NAME} VERSION "0.0.0" LANGUAGES CXX)

find_package(traact_base)
list(APPEND CMAKE_MODULE_PATH ${traact_base_INCLUDE_DIR})

include(traact_default_library_setup)

set(CEREAL_PUBLIC_HEADER
        src/traact/cereal/CerealSpatial.h 
        src/traact/cereal/CerealVision.h 
        )

set(COMPONENT_CEREAL_SRC                
        src/traact/cereal/CerealFileWriterComponent.cpp 
        src/traact/cereal/CerealFileReaderComponent.cpp 
        src/traact/cereal/CerealFileRecorderComponent.cpp 
        src/traact/cereal/CerealFilePlayerComponent.cpp 
        src/traact/cereal/CerealFileReaderWriterComponent.cpp
        )

add_library(${TARGET_NAME} SHARED ${COMPONENT_CEREAL_SRC})

find_package(traact_spatial REQUIRED)
find_package(traact_vision REQUIRED)
find_package(cereal REQUIRED)
find_package(cppfs REQUIRED)

target_link_libraries(${TARGET_NAME} traact_spatial::traact_spatial traact_vision::traact_vision cereal::cereal cppfs::cppfs)

generate_export_header(${TARGET_NAME} EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/traact/traact_cereal_export.h)

target_include_directories(${TARGET_NAME} PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(traact_default_library_final)