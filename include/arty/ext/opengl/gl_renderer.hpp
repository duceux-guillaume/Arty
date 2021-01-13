#ifndef ARTY_EXT_OPENGL_GL_RENDERER_HPP
#define ARTY_EXT_OPENGL_GL_RENDERER_HPP

#include <GL/glew.h>

#include <arty/core/gui/rectangle.hpp>
#include <arty/core/memory.hpp>
#include <unordered_map>

namespace arty {

class GlRenderer {
 private:
  struct MetaData {
    GLuint vbo, vao, ebo;
    GLuint vertexShader, fragmentShader, shaderProgram;
    GLint posAttrib, colAttrib;
  };

  std::unordered_map<Entity, MetaData> _metadatas;

 public:
  void create(Entity const& e, Rectangle const& rect);
  void release(Entity const& e);
  void release();
  void draw(Entity const& e);
  bool has(Entity const& e);
};  // namespace arty

}  // namespace arty

#endif  // !ARTY_EXT_OPENGL_GL_RENDERER_HPP
