cmake_minimum_required (VERSION 3.0)
project (Arty)

###############
## ENV CHECK ##
###############

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
  message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
  message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

###############
## ARTY CORE ##
###############

file(GLOB_RECURSE ARTY_CORE_FILES
  include/arty/core/*
  include/arty/impl/*
  src/arty/core/*
  src/arty/impl/*
  )

add_library(arty_core ${ARTY_CORE_FILES})
target_compile_features(arty_core PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(arty_core PUBLIC -Werror -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
target_include_directories(arty_core
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

##############
## EXTERNAL ##
##############

find_package(OpenGL)

if(OPENGL_FOUND)

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

endif(OPENGL_FOUND)

#############
## ARTY GL ##
#############

if(OPENGL_FOUND)

file(GLOB_RECURSE ARTY_GL_FILES include/arty/ext/* src/arty/ext/*)
add_library(arty_gl ${ARTY_GL_FILES})
target_link_libraries(arty_gl PUBLIC arty_core ${ALL_LIBS})
target_compile_features(arty_gl PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(arty_gl PUBLIC -Werror -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
target_include_directories(arty_gl
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(arty_gl
  SYSTEM PUBLIC
  external/glfw-3.1.2/include/
  external/glew-1.13.0/include/
  external/bullet-2.81-rev2613/src/
)

endif(OPENGL_FOUND)

##############
## ARTY EXE ##
##############

if(OPENGL_FOUND)

add_executable(playground playground/main.cpp)
target_link_libraries(playground PUBLIC arty_core arty_gl)
add_custom_command(
   TARGET playground POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bot_arena${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)

#add_executable(bot_editor bot_editor/main.cpp)
#target_link_libraries(bot_editor PUBLIC arty_core arty_gl)
#add_custom_command(
#   TARGET bot_editor POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bot_editor${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
#)

endif(OPENGL_FOUND)


###########
## GTEST ##
###########

enable_testing()

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
