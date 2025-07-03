cmake_minimum_required(VERSION 3.10)
project(project_name)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the installed lcmware package
find_package(lcmware REQUIRED)

# Find LCM files
file(GLOB LCM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/msg/*.lcm")

# Generate a list of expected output header files
set(GENERATED_HEADERS "")
foreach(LCM_FILE ${LCM_FILES})
    get_filename_component(BASENAME ${LCM_FILE} NAME_WE)
    list(APPEND GENERATED_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/msg/built/${BASENAME}.hpp")
endforeach()

# Custom command to generate LCM files
add_custom_command(
    OUTPUT ${GENERATED_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/msg/built/generated_files.stamp
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/msg/built/"
    COMMAND lcm-gen --lazy --cpp --cpp-hpath "${CMAKE_CURRENT_SOURCE_DIR}/msg/built/" ${LCM_FILES}
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/msg/built/generated_files.stamp
    DEPENDS ${LCM_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating LCM C++ files"
)

# Custom target for LCM generation
add_custom_target(generate_lcm_files 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/msg/built/generated_files.stamp
)

# Define the executable
add_executable(project_name main.cpp)

# Add dependency on LCM file generation
add_dependencies(project_name generate_lcm_files)

# Link with lcmware (interface library)
target_link_libraries(project_name lcmware)

# Include directories for generated headers
target_include_directories(project_name PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/msg/built/)

# Compile features and options
target_compile_features(project_name PRIVATE cxx_std_17)
target_compile_options(project_name PRIVATE -fpermissive)