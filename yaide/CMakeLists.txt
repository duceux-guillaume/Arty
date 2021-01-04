cmake_minimum_required (VERSION 3.18.2)
project (Arty-Yaide)

if(OPENGL_FOUND)
  add_executable(yaide main.cpp)
  target_link_libraries(yaide arty_core arty_gl)
else()
  message( WARNING "OpenGl not found -> Arty-Yaide won't be built")
endif(OPENGL_FOUND)