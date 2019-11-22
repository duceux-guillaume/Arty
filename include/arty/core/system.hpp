#ifndef ECS_H
#define ECS_H

#include <arty/core/blackboard.h>
#include <arty/core/input.h>
#include <arty/core/result.h>

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace arty {

class System {
 public:
  virtual Result process(Ptr<Blackboard> const &board);
  virtual Result process(Ptr<Blackboard> const &board,
                         Ptr<Keyboard> const &keyboard);
  virtual Result init(Ptr<Blackboard> const &board);
  virtual Result init(Ptr<Blackboard> const &board,
                      Ptr<Keyboard> const &keyboard);
  virtual void release();

 protected:
};

};  // namespace arty

#endif  // ECS_H
