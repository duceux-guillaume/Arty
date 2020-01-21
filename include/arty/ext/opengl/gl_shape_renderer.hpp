#ifndef GL_SHAPE_RENDERER_HPP
#define GL_SHAPE_RENDERER_HPP

#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {

class GlShapeRenderer : public IShapeRenderer {
 private:
  unsigned int _shape_array;
  unsigned int _program;
  int _mvp;
  std::unordered_map<Entity, unsigned int> _vbos;

  // IShapeRenderer interface
 public:
  GlShapeRenderer() = default;
  Result init() override;
  void release() override;
  void draw(const Entity &e, const Box &s, const Mat4x4f &model,
            const Mat4x4f &view, const Mat4x4f &proj) override;
  void draw(const Entity &e, const std::vector<Vec3f> &s, const Mat4x4f &model,
            const Mat4x4f &view, const Mat4x4f &proj) override;

 private:
  void import(Entity const &e, const std::vector<Vec3f> &s);
};

}  // namespace arty

#endif  // GL_SHAPE_RENDERER_HPP
