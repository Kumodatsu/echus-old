cmake_minimum_required(VERSION 3.7...3.21)
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(echus
    VERSION     0.1.0
    DESCRIPTION "A simple sound synthesis library."
    LANGUAGES   CXX
)

# dependency libsoundio

add_subdirectory(dependencies/libsoundio)
get_directory_property(LIBSOUNDIO_LIBS
    DIRECTORY  dependencies/libsoundio
    DEFINITION LIBSOUNDIO_LIBS
)
add_compile_definitions(SOUNDIO_STATIC_LIBRARY)

# dependency nanogui

set(NANOGUI_BUILD_SHARED OFF CACHE BOOL " " FORCE)
add_subdirectory(dependencies/nanogui)
add_definitions(${NANOGUI_EXTRA_DEFS})

# echus-lib

add_library(echus-lib STATIC
    projects/echus-lib/src/SoundMachine.cpp
    projects/echus-lib/include/echus/Instruments.hpp
    projects/echus-lib/include/echus/Math.hpp
    projects/echus-lib/include/echus/Music.hpp
    projects/echus-lib/include/echus/SoundMachine.hpp
    projects/echus-lib/include/echus/Synthesis.hpp
)
target_include_directories(echus-lib PUBLIC
    projects/echus-lib/include
    dependencies/libsoundio
)
target_compile_features(echus-lib PUBLIC
    cxx_std_17
)

# echus-exe

add_executable(echus-exe
    projects/echus-exe/src/Main.cpp
    projects/echus-exe/src/GUI.hpp
)
target_include_directories(echus-exe PUBLIC
    projects/echus-exe/src
    dependencies/nanogui
    ${NANOGUI_EXTRA_INCS}
)
target_link_libraries(echus-exe PUBLIC
    echus-lib
    libsoundio_static
    ${LIBSOUNDIO_LIBS}
    nanogui
    ${NANOGUI_EXTRA_LIBS}
)
target_compile_features(echus-exe PUBLIC
    cxx_std_17
)
