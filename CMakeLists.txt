cmake_minimum_required (VERSION 3.0)
project (Arty)

########
## GL ##
########

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

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

include_directories(
        external/AntTweakBar-1.16/include/
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/assimp-3.0.1270/include/
        external/bullet-2.81-rev2613/src/
        .
)

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

# Tutorial 10
add_executable(tutorial10_transparency
        tutorial10_transparency/tutorial10.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        tutorial10_transparency/StandardShading.vertexshader
        tutorial10_transparency/StandardTransparentShading.fragmentshader
)
target_link_libraries(tutorial10_transparency
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial10_transparency PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial10_transparency/")
create_target_launcher(tutorial10_transparency WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial10_transparency/")

# Tutorial 11
add_executable(tutorial11_2d_fonts
        tutorial11_2d_fonts/tutorial11.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/text2D.hpp
        common/text2D.cpp

        tutorial11_2d_fonts/StandardShading.vertexshader
        tutorial11_2d_fonts/StandardShading.fragmentshader
        tutorial11_2d_fonts/TextVertexShader.vertexshader
        tutorial11_2d_fonts/TextVertexShader.fragmentshader

)
target_link_libraries(tutorial11_2d_fonts
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial11_2d_fonts PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial11_2d_fonts/")
create_target_launcher(tutorial11_2d_fonts WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial11_2d_fonts/")

# Tutorial 12
add_executable(tutorial12_extensions
        tutorial12_extensions/tutorial12.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        tutorial12_extensions/StandardShading.vertexshader
        tutorial12_extensions/StandardShading_WithSyntaxErrors.fragmentshader
)
target_link_libraries(tutorial12_extensions
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial12_extensions PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial12_extensions/")
create_target_launcher(tutorial12_extensions WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial12_extensions/")

# Tutorial 13
add_executable(tutorial13_normal_mapping
        tutorial13_normal_mapping/tutorial13.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/text2D.hpp
        common/text2D.cpp
        common/tangentspace.hpp
        common/tangentspace.cpp

        tutorial13_normal_mapping/NormalMapping.vertexshader
        tutorial13_normal_mapping/NormalMapping.fragmentshader
)
target_link_libraries(tutorial13_normal_mapping
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial13_normal_mapping PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial13_normal_mapping/")
create_target_launcher(tutorial13_normal_mapping WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial13_normal_mapping/")


# Tutorial 14
add_executable(tutorial14_render_to_texture
        tutorial14_render_to_texture/tutorial14.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/text2D.hpp
        common/text2D.cpp

        tutorial14_render_to_texture/StandardShadingRTT.vertexshader
        tutorial14_render_to_texture/StandardShadingRTT.fragmentshader
        tutorial14_render_to_texture/Passthrough.vertexshader
        tutorial14_render_to_texture/WobblyTexture.fragmentshader
)
target_link_libraries(tutorial14_render_to_texture
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial14_render_to_texture PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial14_render_to_texture/")
create_target_launcher(tutorial14_render_to_texture WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial14_render_to_texture/")


# Tutorial 15
add_executable(tutorial15_lightmaps
        tutorial15_lightmaps/tutorial15.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        tutorial15_lightmaps/TransformVertexShader.vertexshader
        tutorial15_lightmaps/TextureFragmentShaderLOD.fragmentshader
)
target_link_libraries(tutorial15_lightmaps
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial15_lightmaps PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial15_lightmaps/")
create_target_launcher(tutorial15_lightmaps WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial15_lightmaps/")

# Tutorial 16, simple version
add_executable(tutorial16_shadowmaps_simple
        tutorial16_shadowmaps/tutorial16_SimpleVersion.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        tutorial16_shadowmaps/ShadowMapping_SimpleVersion.vertexshader
        tutorial16_shadowmaps/ShadowMapping_SimpleVersion.fragmentshader
        tutorial16_shadowmaps/DepthRTT.vertexshader
        tutorial16_shadowmaps/DepthRTT.fragmentshader
)
target_link_libraries(tutorial16_shadowmaps_simple
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial16_shadowmaps_simple PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial16_shadowmaps/")
create_target_launcher(tutorial16_shadowmaps_simple WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial16_shadowmaps/")


# Tutorial 16
add_executable(tutorial16_shadowmaps
        tutorial16_shadowmaps/tutorial16.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        tutorial16_shadowmaps/ShadowMapping.vertexshader
        tutorial16_shadowmaps/ShadowMapping.fragmentshader
        tutorial16_shadowmaps/DepthRTT.vertexshader
        tutorial16_shadowmaps/DepthRTT.fragmentshader
        tutorial16_shadowmaps/Passthrough.vertexshader
        tutorial16_shadowmaps/SimpleTexture.fragmentshader
)
target_link_libraries(tutorial16_shadowmaps
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial16_shadowmaps PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial16_shadowmaps/")
create_target_launcher(tutorial16_shadowmaps WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial16_shadowmaps/")

# Tutorial 17
add_executable(tutorial17_rotations
        tutorial17_rotations/tutorial17.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp
        common/quaternion_utils.cpp
        common/quaternion_utils.hpp

        tutorial17_rotations/StandardShading.vertexshader
        tutorial17_rotations/StandardShading.fragmentshader
)
target_link_libraries(tutorial17_rotations
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
)
# Xcode and Visual working directories
set_target_properties(tutorial17_rotations PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial17_rotations/")
create_target_launcher(tutorial17_rotations WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial17_rotations/")

# User playground
add_executable(playground
        playground/playground.cpp
        common/shader.cpp
        common/shader.hpp
)
target_link_libraries(playground
        ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(playground PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/playground/")
create_target_launcher(playground WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/playground/")



# Misc 5, with glReadPixels
add_executable(misc05_picking_slow_easy
        misc05_picking/misc05_picking_slow_easy.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        misc05_picking/StandardShading.vertexshader
        misc05_picking/StandardShading.fragmentshader
        misc05_picking/Picking.vertexshader
        misc05_picking/Picking.fragmentshader
)
target_link_libraries(misc05_picking_slow_easy
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
)
# Xcode and Visual working directories
set_target_properties(misc05_picking_slow_easy PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/")
create_target_launcher(misc05_picking_slow_easy WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/")

# Misc 5, with custom ray-box intersection
add_executable(misc05_picking_custom
        misc05_picking/misc05_picking_custom.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        misc05_picking/StandardShading.vertexshader
        misc05_picking/StandardShading.fragmentshader
)
target_link_libraries(misc05_picking_custom
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
)
# Xcode and Visual working directories
set_target_properties(misc05_picking_custom PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/")
create_target_launcher(misc05_picking_custom WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/")

# Misc 5, with Bullet Physics
add_executable(misc05_picking_BulletPhysics
        misc05_picking/misc05_picking_BulletPhysics.cpp
        common/shader.cpp
        common/shader.hpp
        common/controls.cpp
        common/controls.hpp
        common/texture.cpp
        common/texture.hpp
        common/objloader.cpp
        common/objloader.hpp
        common/vboindexer.cpp
        common/vboindexer.hpp

        misc05_picking/StandardShading.vertexshader
        misc05_picking/StandardShading.fragmentshader
)
target_link_libraries(misc05_picking_BulletPhysics
        ${ALL_LIBS}
        ANTTWEAKBAR_116_OGLCORE_GLFW
        BulletDynamics
        BulletCollision
        LinearMath
)
# Xcode and Visual working directories
set_target_properties(misc05_picking_BulletPhysics PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/")
create_target_launcher(misc05_picking_BulletPhysics WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/")



add_executable(tutorial18_billboards
        tutorial18_billboards_and_particles/tutorial18_billboards.cpp
        common/shader.cpp
        common/shader.hpp
        common/texture.cpp
        common/texture.hpp
        common/controls.cpp
        common/controls.hpp
        tutorial18_billboards_and_particles/Billboard.fragmentshader
        tutorial18_billboards_and_particles/Billboard.vertexshader
)

target_link_libraries(tutorial18_billboards
        ${ALL_LIBS}
)

# Xcode and Visual working directories
set_target_properties(tutorial18_billboards PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial18_billboards_and_particles/")
create_target_launcher(tutorial18_billboards WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial18_billboards_and_particles/")

add_executable(tutorial18_particles
        tutorial18_billboards_and_particles/tutorial18_particles.cpp
        common/shader.cpp
        common/shader.hpp
        common/texture.cpp
        common/texture.hpp
        common/controls.cpp
        common/controls.hpp
        tutorial18_billboards_and_particles/Particle.fragmentshader
        tutorial18_billboards_and_particles/Particle.vertexshader
)

target_link_libraries(tutorial18_particles
        ${ALL_LIBS}
)

# Xcode and Visual working directories
set_target_properties(tutorial18_particles PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial18_billboards_and_particles/")
create_target_launcher(tutorial18_particles WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial18_billboards_and_particles/")

##########
## ARTY ##
##########

file(GLOB_RECURSE ARTY_MATH_FILES include/arty/* src/arty/*)
message(info ${ARTY_MATH_FILES})
add_library(arty ${ARTY_MATH_FILES})
target_link_libraries(arty ${ALL_LIBS} assimp)
target_compile_features(arty PRIVATE cxx_std_17)

include_directories(include)

#add_executable(artyshell src/bin/artyshell.cpp)
#target_link_libraries(artyshell arty)

add_executable(bot_arena
        bot_arena/bot_arena.cpp
)
target_link_libraries(bot_arena
        arty ${ALL_LIBS}
)

################
## POST BUILD ##
################

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET bot_arena POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bot_arena${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)
add_custom_command(
   TARGET tutorial10_transparency POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial10_transparency${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial10_transparency/"
)
add_custom_command(
   TARGET tutorial11_2d_fonts POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial11_2d_fonts${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial11_2d_fonts/"
)
add_custom_command(
   TARGET tutorial12_extensions POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial12_extensions${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial12_extensions/"
)
add_custom_command(
   TARGET tutorial13_normal_mapping POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial13_normal_mapping${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial13_normal_mapping/"
)
add_custom_command(
   TARGET tutorial14_render_to_texture POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial14_render_to_texture${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial14_render_to_texture/"
)
 add_custom_command(
   TARGET tutorial15_lightmaps POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial15_lightmaps${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial15_lightmaps/"
)
add_custom_command(
   TARGET tutorial16_shadowmaps_simple POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial16_shadowmaps_simple${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial16_shadowmaps/"
)
add_custom_command(
   TARGET tutorial16_shadowmaps POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial16_shadowmaps${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial16_shadowmaps/"
)
add_custom_command(
   TARGET tutorial17_rotations POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial17_rotations${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial17_rotations/"
)
add_custom_command(
   TARGET tutorial18_billboards POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial18_billboards${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial18_billboards_and_particles/"
)
add_custom_command(
   TARGET tutorial18_particles POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial18_particles${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial18_billboards_and_particles/"
)
add_custom_command(
   TARGET playground POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/playground${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/playground/"
)
add_custom_command(
   TARGET misc05_picking_slow_easy POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/misc05_picking_slow_easy${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/"
)
add_custom_command(
   TARGET misc05_picking_custom POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/misc05_picking_custom${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/"
)
add_custom_command(
   TARGET misc05_picking_BulletPhysics POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/misc05_picking_BulletPhysics${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/misc05_picking/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

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
