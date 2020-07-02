project (libMiniOBJ)
add_library(libMiniOBJ INTERFACE)
target_include_directories(libMiniOBJ INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/miniOBJ>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS libMiniOBJ)


if (${MINI_BUILD_MINIFIER})
    FILE(GLOB_RECURSE SOURCES src/*.cpp)
    add_executable(miniOBJ ${SOURCES})
    target_link_libraries(miniOBJ libMiniOBJ)

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    install(TARGETS miniOBJ)
endif()