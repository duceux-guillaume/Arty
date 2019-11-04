#ifndef GL_SHAPE_RENDERER_HPP
#define GL_SHAPE_RENDERER_HPP

#include <GL/glew.h>

#include <arty/impl/collision_rendering_system.hpp>

namespace arty {

class GlShapeRenderer : public IShapeRenderer {
 private:
  std::unordered_map<Entity, GLuint> _vbos;
  GLuint _shape_array;
  GLuint _program;
  GLuint _mvp;

  // IShapeRenderer interface
 public:
  GlShapeRenderer() = default;
  Result init() override;
  void release() override;
  void import(const Entity &e, const Shape3f &s) override;
  void draw(const Entity &e, const Shape3f &s, const Mat4x4f &model,
            const Mat4x4f &view, const Mat4x4f &proj) override;
};

}  // namespace arty

#endif  // GL_SHAPE_RENDERER_HPP
