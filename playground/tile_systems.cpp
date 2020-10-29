#include <arty/impl/camera_system.hpp>

#include "tile_systems.hpp"

void RandomBoardInitSystem::reset(Ptr<Memory> const& mem) {
  mem->clear();

  TileBoard board(10, 5);
  mem->write(board);

  auto time_seed = static_cast<long unsigned>(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  static auto type_rand = std::bind(std::uniform_int_distribution<int>{0, 1},
                                    std::default_random_engine{time_seed});
  static auto endpoint_rand =
      std::bind(std::uniform_int_distribution<uint8_t>{0, 3},
                std::default_random_engine{time_seed});

  for (uint8_t i = 0; i < board.rows; ++i) {
    for (uint8_t j = 0; j < board.cols; ++j) {
      auto entity = mem->createEntity("tile");
      Vec2ui8 tile_pos(i, j);
      mem->write(entity, tile_pos);
      mem->write(entity, board.tile2box(tile_pos));
      mem->write(entity, board.tile2tf(tile_pos));
      auto type = type_rand();
      switch (type) {
        case 1: {
          // Conductor
          auto e1 = endpoint_rand();
          auto e2 = endpoint_rand();
          mem->write(entity, TileWire(e1, e2));
          break;
        }
        default: {
          // Isolant
          break;
        }
      }
    }
  }
}

Result TileRenderingSystem::process(const Ptr<Memory>& mem) {
  Camera cam;
  return_if_error(mem->read(cam));

  TileBoard board;
  return_if_error(mem->read(board));

  if (mem->count<TileWire>()) {  // Tile wiring
    auto work = [=](Entity const& e, Vec2ui8 const& pos,
                    TileWire const& wire) -> Result {
      _renderer->draw(e, board.wire2segments(pos, wire),
                      board.tile2tf(pos).toMat(), cam.view(), cam.projection());
      return ok();
    };
    mem->process<Vec2ui8, TileWire>(work);
  }

  return ok();
}