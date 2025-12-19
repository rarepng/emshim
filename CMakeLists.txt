cmake_minimum_required(VERSION 4.0)
project(emshim LANGUAGES CXX)


include(ExternalProject)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    "$<$<CONFIG:Debug>:-Wall>"
    "$<$<CONFIG:Debug>:-Wextra>"
    "$<$<CONFIG:Debug>:-Wunused-variable>"
)

set(submodules_path
        ${PROJECT_SOURCE_DIR}/sub
)

if(UNIX)
set(SHELL_CMD bash)
set(SEP ":")
set(STATIC_LIB_EXT a)
else()
set(SHELL_CMD pwsh)  #powershell 7
#set(SHELL_CMD powershell) #default for windows
set(EXT ".exe")
set(SEP ";")
set(STATIC_LIB_EXT lib)
endif()


set(BUILD_SHARED_LIBS OFF CACHE BOOL "set" FORCE)



file(GLOB PROJECT_HEADERS "include/*.hpp")
file(GLOB PROJECT_SOURCES "src/main.cpp" "src/*/*.cpp")
set(OTHER_SOURCES
)

add_executable(emshim ${OTHER_SOURCES} ${PROJECT_SOURCES} ${PROJECT_HEADERS})



if(UNIX)
set_target_properties(emshim PROPERTIES CMAKE_CXX_FLAGS '-static')
else()
set_target_properties(emshim PROPERTIES CMAKE_CXX_FLAGS '-static\ -mwindows')
endif()



if(UNIX)
target_link_libraries(emshim
    PRIVATE
        c
        pthread
)
else()
target_link_libraries(emshim
    PRIVATE
        DbgHelp
        winmm
        iphlpapi
)
endif()

target_include_directories(emshim
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)



install(TARGETS emshim
    RUNTIME DESTINATION .
)


