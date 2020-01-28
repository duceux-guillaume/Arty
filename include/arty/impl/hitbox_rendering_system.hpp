#ifndef HITBOX_RENDERING_SYSTEM_HPP
#define HITBOX_RENDERING_SYSTEM_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/system.hpp>

namespace arty {

class IShapeRenderer {
 public:
  virtual ~IShapeRenderer();
  virtual Result init() = 0;
  virtual void draw(const Entity& e, const AABox3f& s, const Mat4x4f& model,
                    const Mat4x4f& view, const Mat4x4f& proj) = 0;
  virtual void draw(const Entity& e, const OBB3f& s, const Mat4x4f& model,
                    const Mat4x4f& view, const Mat4x4f& proj) = 0;
  virtual void draw(const Entity& e, const std::vector<Vec3f>& s,
                    const Mat4x4f& model, const Mat4x4f& view,
                    const Mat4x4f& proj) = 0;
  virtual void release() = 0;
};

class HitBoxRenderingSystem : public System {
 public:
  static constexpr const char* DRAW_AABB = "hitbox";
  static constexpr const char* DRAW_OBB = "obb";

 public:
  HitBoxRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
  Result init(const Ptr<Memory>& board) override;
};

}  // namespace arty

#endif  // HITBOX_RENDERING_SYSTEM_HPP
