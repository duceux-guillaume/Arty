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

void GlShapeRenderer::draw(const Property<Shape3f>& s, const Mat4x4f& model,
                           const Mat4x4f& view, const Mat4x4f& proj) {
  glUseProgram(_program);
  Mat4x4f mvp = proj * view * model;
  glUniformMatrix4fv(_mvp, 1, GL_FALSE, mvp.transpose().ptr());
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbos[s.entity]);
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );
  glDrawArrays(GL_LINE_STRIP, 0, s.value.pts().size());
}

void GlShapeRenderer::release() {}

void GlShapeRenderer::import(const Property<Shape3f>& s) {
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, s.value.pts().size() * sizeof(Vec3f),
               s.value.pts().data(), GL_STATIC_DRAW);
  _vbos[s.entity] = vbo;
}

}  // namespace arty
