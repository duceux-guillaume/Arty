#include <arty/core/system.hpp>

namespace arty {

Result System::process(Ptr<Memory> const&) { return ok(); }

Result System::process(Ptr<Memory> const& board, Ptr<Keyboard> const& keyboard,
                       Ptr<Mouse> const& /*mouse*/) {
  return process(board, keyboard);
}

Result System::process(Ptr<Memory> const& board,
                       Ptr<Keyboard> const& /*keyboard*/) {
  return process(board);
}

Result System::init(Ptr<Memory> const&) { return ok(); }

Result System::init(Ptr<Memory> const& board,
                    Ptr<Keyboard> const& /*keyboard*/) {
  return init(board);
}

void System::release() {}

}  // namespace arty
