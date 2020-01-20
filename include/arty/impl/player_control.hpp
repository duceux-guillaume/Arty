#ifndef PLAYER_CONTROL_HPP
#define PLAYER_CONTROL_HPP

#include <arty/core/system.hpp>

namespace arty {

class PlayerControlSystem : public System {
 private:
  InputEvent _up;

 public:
  // System interface
 public:
  Result process(const Ptr<Memory> &board,
                 Ptr<Keyboard> const &keyboard) override;
  Result init(const Ptr<Memory> &board,
              Ptr<Keyboard> const &keyboard) override;
  void release() override;
};

}  // namespace arty

#endif  // PLAYER_CONTROL_HPP
