#ifndef ECS_H
#define ECS_H

#include <arty/core/input.h>
#include <arty/core/result.h>

#include <arty/core/blackboard.hpp>

namespace arty {

class System {
 public:
  virtual ~System() {}

  virtual Result process(Ptr<Memory> const &board);
  virtual Result process(Ptr<Memory> const &board,
                         Ptr<Keyboard> const &keyboard);
  virtual Result init(Ptr<Memory> const &board);
  virtual Result init(Ptr<Memory> const &board, Ptr<Keyboard> const &keyboard);
  virtual void release();

 protected:
};

};  // namespace arty

#endif  // ECS_H
