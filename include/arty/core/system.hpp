#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <arty/core/input.hpp>
#include <arty/core/memory.hpp>
#include <arty/core/result.hpp>

namespace arty {

class System {
 public:
  virtual ~System() {}

  virtual Result process(Ptr<Memory> const& board,
                         Ptr<Keyboard> const& keyboard,
                         Ptr<Mouse> const& mouse);
  virtual Result process(Ptr<Memory> const& board,
                         Ptr<Keyboard> const& keyboard);
  virtual Result process(Ptr<Memory> const& board);
  virtual Result init(Ptr<Memory> const& board, Ptr<Keyboard> const& keyboard,
                      Ptr<Mouse> const& mouse);
  virtual Result init(Ptr<Memory> const& board, Ptr<Keyboard> const& keyboard);
  virtual Result init(Ptr<Memory> const& board);
  virtual void release();

 protected:
};

};  // namespace arty

#endif  // SYSTEM_HPP
