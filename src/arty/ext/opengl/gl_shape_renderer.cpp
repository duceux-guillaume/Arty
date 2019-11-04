#include <arty/ext/opengl/gl_loader.h>

#include <arty/ext/opengl/gl_shape_renderer.hpp>

namespace arty {
Result GlShapeRenderer::init() {
  glGenVertexArrays(1, &_shape_array);
  glBindVertexArray(_shape_array);

  _program = LoadShaders("../shaders/SimpleTransform.vertexshader",
                         "../shaders/SingleColor.fragmentshader");
  if (_program == 0) {
    return error("error opening program file");
  }
  _mvp = glGetUniformLocation(_program, "MVP");

  return ok();
}

void GlShapeRenderer::draw(Entity const& e, const Shape3f& s,
                           const Mat4x4f& model, const Mat4x4f& view,
                           const Mat4x4f& proj) {
  import(e, s);
  glUseProgram(_program);
  Mat4x4f mvp = proj * view * model;
  glUniformMatrix4fv(_mvp, 1, GL_FALSE, mvp.transpose().ptr());
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbos[e]);
  glBufferData(GL_ARRAY_BUFFER, s.pts().size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, s.pts().size() * sizeof(Vec3f),
                  s.pts().data());

  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );
  glDrawArrays(GL_LINES, 0, s.pts().size());
}

void GlShapeRenderer::release() {}

void GlShapeRenderer::import(Entity const& e, Shape3f const& s) {
  if (_vbos.count(e) > 0) {
    return;
  }
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, s.pts().size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
  _vbos[e] = vbo;
}

}  // namespace arty
