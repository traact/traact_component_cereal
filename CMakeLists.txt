cmake_minimum_required(VERSION 3.16)

if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
else ()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CONAN_TRAACT_RUN_ENV_ROOT}/cmake)

project(${CONAN_PACKAGE_NAME}_PROJECT VERSION ${CONAN_PACKAGE_VERSION})

include(traact_default_library_setup)

set(COMPONENT_CEREAL_SRC
        src/traact/traact_serialization_export.h
        src/traact/cereal/CerealSpatial.cpp src/traact/cereal/CerealSpatial.h src/traact/cereal/CerealVision.cpp src/traact/cereal/CerealVision.h src/traact/cereal/CerealFileWriterComponent.cpp src/traact/cereal/CerealFileReaderComponent.cpp src/traact/cereal/CerealFileRecorderComponent.cpp src/traact/cereal/CerealFilePlayerComponent.cpp)

add_library(${CONAN_PACKAGE_NAME} ${COMPONENT_CEREAL_SRC})

conan_target_link_libraries(${CONAN_PACKAGE_NAME})

target_link_libraries(dataflow_test_test PRIVATE ${CONAN_PACKAGE_NAME})