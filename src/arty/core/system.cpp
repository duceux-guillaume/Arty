#include <arty/core/system.hpp>

namespace arty {

Result System::process(Ptr<Memory> const&) { return ok(); }

Result System::process(Ptr<Memory> const& board,
                       Ptr<Keyboard> const& /*keyboard*/) {
  return process(board);
}

Result System::init(Ptr<Memory> const&) { return ok(); }

Result System::init(const Ptr<Memory>& board,
                    const Ptr<Keyboard>& /*keyboard*/) {
  return init(board);
}

void System::release() {}

}  // namespace arty
