#include <arty/impl/engine.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <chrono>
#include <random>

using namespace arty;

struct TileWire {
  TileWire(uint8_t i, uint8_t j) : endpoint(i, j) {}
  Vec2u8 endpoint;
};

struct TileBoard {
  uint8_t rows;
  uint8_t cols;
  Vec3f tileHalfLength;

  TileBoard() : TileBoard(0, 0) {}
  TileBoard(uint8_t r, uint8_t c)
      : rows(r), cols(c), tileHalfLength(0.4f, 0.4f, 0.1f) {}

  AABox3f tile2box(Vec2u8 const& /*tile*/) const {
    AABox3f box(Vec3f(0.f, 0.f, 0.f), tileHalfLength);
    return box;
  }

  Tf3f tile2tf(Vec2u8 const& tile) const {
    float x = tile.x() - rows / 2;
    float y = tile.y() - cols / 2;
    Tf3f tf(Vec3f(x, y, 0));
    return tf;
  }

  std::vector<Vec3f> wire2segments(Vec2u8 const& /*tile*/,
                                   TileWire const& wire) const {
    std::vector<Vec3f> res;
    // Middle
    float mx = 0.f;
    float my = 0.f;
    res.emplace_back(mx, my, 0);
    // First endpoint
    {
      float e1x = 0.f;
      float e1y = 0.f;
      if (wire.endpoint.x() == 0) {  // Left
        e1y += tileHalfLength.y();
      } else if (wire.endpoint.x() == 1) {  // bottom
        e1x += tileHalfLength.x();
      } else if (wire.endpoint.x() == 2) {  // right
        e1y -= tileHalfLength.y();
      } else if (wire.endpoint.x() == 3) {  // top
        e1x += tileHalfLength.x();
      }
      res.emplace_back(e1x, e1y, 0);
    }
    res.emplace_back(mx, my, 0);
    // second endpoint
    {
      float e1x = 0.f;
      float e1y = 0.f;
      if (wire.endpoint.y() == 0) {  // Left
        e1y += tileHalfLength.y();
      } else if (wire.endpoint.y() == 1) {  // bottom
        e1x += tileHalfLength.x();
      } else if (wire.endpoint.y() == 2) {  // right
        e1y -= tileHalfLength.y();
      } else if (wire.endpoint.y() == 3) {  // top
        e1x += tileHalfLength.x();
      }
      res.emplace_back(e1x, e1y, 0);
    }
    return res;
  }
};

class RandomBoardInitSystem : public System {
 private:
  Event _reset;

  void reset(Ptr<Memory> const& mem);

 public:
  RandomBoardInitSystem() : _reset("RESET") {}

  Result process(const Ptr<Memory>& mem,
                 Ptr<InputManager> const& inputs) override {
    if (inputs->pop(_reset)) {
      reset(mem);
    }
    return ok();
  }
  Result init(const Ptr<Memory>& mem,
              Ptr<InputManager> const& inputs) override {
    if (!inputs->attach(Keyboard::SPACE, Device::Action::PRESS, _reset)) {
      return error("couldn't register event");
    }
    reset(mem);
    return ok();
  }
};

class TileRenderingSystem : public System {
 public:
  TileRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;

  Result init(const Ptr<Memory>& /*board*/) override {
    return_if_error(_renderer->init());
    return ok();
  }
};
