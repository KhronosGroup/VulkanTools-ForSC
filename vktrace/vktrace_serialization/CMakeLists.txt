project(vktrace_serialization)
cmake_minimum_required(VERSION 2.8)

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/lvl_genvk.py -registry ${SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_vk.proto)
execute_process(COMMAND protoc -I=${GENERATED_FILES_DIR} --cpp_out=${GENERATED_FILES_DIR} ${GENERATED_FILES_DIR}/vktrace_vk.proto )
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/lvl_genvk.py -registry ${SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_serialization.h)
execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SCRIPTS_DIR}/lvl_genvk.py -registry ${SCRIPTS_DIR}/vk.xml -o ${GENERATED_FILES_DIR} vktrace_serialization.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../)

include_directories(
    ${PROJECT_BINARY_DIR}/../..
    ${SRC_DIR}/vktrace_serialization
    ${SRC_DIR}/thirdparty
)

set (SRC_LIST
     ${GENERATED_FILES_DIR}/vktrace_serialization.cpp
)

set_source_files_properties( ${SRC_LIST} PROPERTIES LANGUAGE CXX)

file( GLOB_RECURSE HDRS *.[h|inl] )

if (NOT MSVC)
    add_cpp_compiler_flag("-fPIC -std=c++11")
endif()

add_library(${PROJECT_NAME} STATIC ${SRC_LIST} ${CXX_SRC_LIST} ${HDRS})

add_dependencies(${PROJECT_NAME} generate_helper_files)

build_options_finalize()

set_target_properties(vktrace_common PROPERTIES LINKER_LANGUAGE C)
