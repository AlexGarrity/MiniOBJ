project (libMiniOBJ)
add_library(libMiniOBJ INTERFACE)
target_include_directories(libMiniOBJ INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/miniOBJ>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
install(TARGETS libMiniOBJ)


if (MINIOBJ_BUILD_MINFIER)
    message("Building Minifier")
    FILE(GLOB_RECURSE SOURCES src/*.cpp)
    add_executable(miniOBJ ${SOURCES})
    if (MINIOBJ_VERBOSE)
        message("Verbose logging enabled")
        target_compile_definitions(miniOBJ PRIVATE MINIOBJ_VERBOSE)
    endif()
    target_include_directories(miniOBJ PRIVATE include)
    target_link_libraries(miniOBJ libMiniOBJ)

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    install(TARGETS miniOBJ)
endif()