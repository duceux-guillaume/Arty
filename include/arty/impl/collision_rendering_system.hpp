#ifndef COLLISION_RENDERING_SYSTEM_HPP
#define COLLISION_RENDERING_SYSTEM_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/system.hpp>

namespace arty {

class IShapeRenderer {
 public:
  virtual Result init() = 0;
  virtual void draw(Property<Shape3f> const &s, const Mat4x4f &model,
                    const Mat4x4f &view, const Mat4x4f &proj) = 0;
  virtual void release() = 0;
  virtual void import(Property<Shape3f> const &s) = 0;
};

class CollisionRenderingSystem : public System {
 public:
  static constexpr const char *IMPORT_PROP = "shape2import";
  static constexpr const char *DRAW_PROP = "shape";

 public:
  CollisionRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Blackboard> &board) override;
  Result init(const Ptr<Blackboard> &board) override;
  void release() override;
};
}  // namespace arty

#endif  // COLLISION_RENDERING_SYSTEM_HPP
