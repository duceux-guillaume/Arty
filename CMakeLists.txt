cmake_minimum_required (VERSION 3.0)
project (Arty)

##############
## EXTERNAL ##
##############

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
        message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
        message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies
add_subdirectory (external)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

##########
## ARTY ##
##########

file(GLOB_RECURSE ARTY_MATH_FILES include/arty/* src/arty/*)
message(info ${ARTY_MATH_FILES})
add_library(arty ${ARTY_MATH_FILES})
target_link_libraries(arty PRIVATE ${ALL_LIBS})
target_compile_features(arty PRIVATE cxx_std_17)
target_compile_options(arty PRIVATE -Werror -Wall -Wextra)
target_include_directories(arty
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(arty
  SYSTEM PUBLIC
  external/glfw-3.1.2/include/
  external/glew-1.13.0/include/
  external/bullet-2.81-rev2613/src/
)


add_executable(bot_arena bot_arena/bot_arena.cpp)
target_link_libraries(bot_arena PUBLIC arty ${ALL_LIBS})

################
## POST BUILD ##
################

add_custom_command(
   TARGET bot_arena POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bot_arena${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)

###########
## GTEST ##
###########

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(external/googletest EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_subdirectory(test)
