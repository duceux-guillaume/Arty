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

void GlShapeRenderer::draw(const Entity& e, const AABox3f& box,
                           const Mat4x4f& model, const Mat4x4f& view,
                           const Mat4x4f& proj) {
  std::vector<Vec3f> lines;
  // sup part
  Vec3f prev = box.center() + box.halfLength();
  Vec3f next = box.center() + flip(box.halfLength(), Vec3f(-1.f, 1.f, 1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = next;
  next = box.center() + flip(box.halfLength(), Vec3f(-1.f, -1.f, 1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = next;
  next = box.center() + flip(box.halfLength(), Vec3f(1.f, -1.f, 1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = next;
  next = box.center() + flip(box.halfLength(), Vec3f(1.f, 1.f, 1.f));
  lines.push_back(prev);
  lines.push_back(next);
  // inf part
  prev = box.center() + flip(box.halfLength(), Vec3f(1.f, 1.f, -1.f));
  next = box.center() + flip(box.halfLength(), Vec3f(-1.f, 1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = next;
  next = box.center() + flip(box.halfLength(), Vec3f(-1.f, -1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = next;
  next = box.center() + flip(box.halfLength(), Vec3f(1.f, -1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = next;
  next = box.center() + flip(box.halfLength(), Vec3f(1.f, 1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  // middle part
  prev = box.center() + box.halfLength();
  next = box.center() + flip(box.halfLength(), Vec3f(1.f, 1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = box.center() + flip(box.halfLength(), Vec3f(1.f, -1.f, 1.f));
  next = box.center() + flip(box.halfLength(), Vec3f(1.f, -1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = box.center() + flip(box.halfLength(), Vec3f(-1.f, 1.f, 1.f));
  next = box.center() + flip(box.halfLength(), Vec3f(-1.f, 1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  prev = box.center() + flip(box.halfLength(), Vec3f(-1.f, -1.f, 1.f));
  next = box.center() + flip(box.halfLength(), Vec3f(-1.f, -1.f, -1.f));
  lines.push_back(prev);
  lines.push_back(next);
  draw(e, lines, model, view, proj);
}

void GlShapeRenderer::draw(const Entity& e, const OBB3f& b,
                           const Mat4x4f& model, const Mat4x4f& view,
                           const Mat4x4f& proj) {
  std::vector<Vec3f> l;
  // sup part
  Vec3f prev = b.center() * b.halfLength();
  Vec3f next = b.center() * flip(b.halfLength(), Vec3f(-1.f, 1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() * flip(b.halfLength(), Vec3f(-1.f, -1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() * flip(b.halfLength(), Vec3f(1.f, -1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() * flip(b.halfLength(), Vec3f(1.f, 1.f, 1.f));
  l.push_back(prev);
  l.push_back(next);
  // inf part
  prev = b.center() * flip(b.halfLength(), Vec3f(1.f, 1.f, -1.f));
  next = b.center() * flip(b.halfLength(), Vec3f(-1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() * flip(b.halfLength(), Vec3f(-1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() * flip(b.halfLength(), Vec3f(1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = next;
  next = b.center() * flip(b.halfLength(), Vec3f(1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  // middle part
  prev = b.center() * b.halfLength();
  next = b.center() * flip(b.halfLength(), Vec3f(1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = b.center() * flip(b.halfLength(), Vec3f(1.f, -1.f, 1.f));
  next = b.center() * flip(b.halfLength(), Vec3f(1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = b.center() * flip(b.halfLength(), Vec3f(-1.f, 1.f, 1.f));
  next = b.center() * flip(b.halfLength(), Vec3f(-1.f, 1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  prev = b.center() * flip(b.halfLength(), Vec3f(-1.f, -1.f, 1.f));
  next = b.center() * flip(b.halfLength(), Vec3f(-1.f, -1.f, -1.f));
  l.push_back(prev);
  l.push_back(next);
  draw(e, l, model, view, proj);
}

void GlShapeRenderer::draw(const Entity& e, const Sphere3f& s,
                           const Mat4x4f& model, const Mat4x4f& view,
                           const Mat4x4f& proj) {
  std::vector<Vec3f> lines;
  static std::size_t num_segments = 50;
  float angle = M_PI / num_segments * 2.f;
  float radius = std::sqrt(s.sqrRadius());
  for (std::size_t jj = 0; jj < num_segments; ++jj) {
    float phy = jj * M_PI / num_segments - M_PI / 2.f;
    float z = radius * std::sin(phy);
    float theta = 0.f * angle;
    float x = radius * std::cos(theta) * std::cos(phy);
    float y = radius * std::sin(theta) * std::cos(phy);
    Vec3f first(x, y, z), prev(x, y, z), next(x, y, z);
    for (std::size_t ii = 1; ii < num_segments; ++ii) {
      theta = ii * angle;
      x = radius * std::cos(theta) * std::cos(phy);
      y = radius * std::sin(theta) * std::cos(phy);
      next = Vec3f(x, y, z);
      lines.push_back(prev);
      lines.push_back(next);
      prev = next;
    }
    lines.push_back(prev);
    lines.push_back(first);
  }
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
