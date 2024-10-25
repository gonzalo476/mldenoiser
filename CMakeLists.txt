cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(MLDenoiser)

# Build configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX /EHsc ${TORCH_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Options for Nuke versions
option(NUKE_11.3 "Nuke 11.3" OFF)
option(NUKE_12.0 "Nuke 12.0" OFF)
option(NUKE_12.1 "Nuke 12.1" ON)
option(NUKE_12.2 "Nuke 12.2" OFF)

# Path configuration for Nuke
if (NUKE_11.3)
    include_directories("C:/Program Files/Nuke11.3v4/include")
    link_directories("C:/Program Files/Nuke11.3v4")
endif()

if (NUKE_12.0)
    include_directories("C:/Program Files/Nuke12.0v1/include")
    link_directories("C:/Program Files/Nuke12.0v1")
endif()

if (NUKE_12.1)
    include_directories("C:/Program Files/Nuke12.1v2/include")
    link_directories("C:/Program Files/Nuke12.1v2")
endif()

if (NUKE_12.2)
    include_directories("C:/Program Files/Nuke12.2v11/include")
    link_directories("C:/Program Files/Nuke12.2v11")
endif()

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include LibTorch
find_package(Torch REQUIRED)

# Define the library
add_library(${CMAKE_PROJECT_NAME} SHARED 
    src/mldenoiser.cpp
)

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} 
    Qt5::Widgets 
    ${OpenCV_LIBS} 
    "${TORCH_LIBRARIES}"
    DDImage
)

# Copy DLLs for Windows as per PyTorch recommendation
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
endif()

# Automatic installation of the plugin in the Nuke directory
if(WIN32)
  set(PLUGIN_INSTALL_DIR "C:/Users/rojas/.nuke")
else()
  set(PLUGIN_INSTALL_DIR "/unknown-os")
endif()

option(AUTO_DEPLOY "Install the plugin" ON)
if(AUTO_DEPLOY)
    message("installing...")
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            ${PLUGIN_INSTALL_DIR}/${CMAKE_PROJECT_NAME}/$<TARGET_FILE_NAME:${CMAKE_PROJECT_NAME}>
    )
    install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "${PLUGIN_INSTALL_DIR}/${CMAKE_PROJECT_NAME}/")
    message("install complete!")
endif()
