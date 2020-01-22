#include <GL/glew.h>

#include <arty/ext/opengl/gl_loader.hpp>
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

void GlShapeRenderer::draw(Entity const& e, const std::vector<Vec3f>& s,
                           const Mat4x4f& model, const Mat4x4f& view,
                           const Mat4x4f& proj) {
  import(e, s);
  glUseProgram(_program);
  Mat4x4f mvp = proj * view * model;
  glUniformMatrix4fv(_mvp, 1, GL_FALSE, mvp.transpose().ptr());
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbos[e]);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(s.size() * sizeof(Vec3f)), nullptr,
               GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  static_cast<GLsizeiptr>(s.size() * sizeof(Vec3f)), s.data());

  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        nullptr    // array buffer offset
  );
  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(s.size()));
}

static Vec3f flip(Vec3f const& l, Vec3f const& r) {
  return l.apply(r, [](float l, float r) { return l * r; });
}

static void boxToLines(AABox3f const& b, std::vector<Vec3f>& l) {
  l.clear();
  // sup part
  Vec3f prev = b.center() + b.halfLength();
  Vec3f next = b.center() + flip(b.halfLength(), Vec3f(-1.f, 1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() + flip(b.halfLength(), Vec3f(-1.f, -1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() + flip(b.halfLength(), Vec3f(1.f, -1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() + flip(b.halfLength(), Vec3f(1.f, 1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  // inf part
  prev = b.center() + flip(b.halfLength(), Vec3f(1.f, 1.f, -1.f));
  next = b.center() + flip(b.halfLength(), Vec3f(-1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() + flip(b.halfLength(), Vec3f(-1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() + flip(b.halfLength(), Vec3f(1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() + flip(b.halfLength(), Vec3f(1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  // middle part
  prev = b.center() + b.halfLength();
  next = b.center() + flip(b.halfLength(), Vec3f(1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = b.center() + flip(b.halfLength(), Vec3f(1.f, -1.f, 1.f));
  next = b.center() + flip(b.halfLength(), Vec3f(1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = b.center() + flip(b.halfLength(), Vec3f(-1.f, 1.f, 1.f));
  next = b.center() + flip(b.halfLength(), Vec3f(-1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = b.center() + flip(b.halfLength(), Vec3f(-1.f, -1.f, 1.f));
  next = b.center() + flip(b.halfLength(), Vec3f(-1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
}

void GlShapeRenderer::draw(const Entity& e, const AABox3f& s,
                           const Mat4x4f& model, const Mat4x4f& view,
                           const Mat4x4f& proj) {
  std::vector<Vec3f> lines;
  boxToLines(s, lines);
  draw(e, lines, model, view, proj);
}

void GlShapeRenderer::release() {}

void GlShapeRenderer::import(Entity const& e, std::vector<Vec3f> const& s) {
  if (_vbos.count(e) > 0) {
    return;
  }
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(s.size() * sizeof(Vec3f)), nullptr,
               GL_STREAM_DRAW);
  _vbos[e] = vbo;
}

}  // namespace arty
