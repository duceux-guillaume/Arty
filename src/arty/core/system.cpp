#include <arty/core/system.hpp>

namespace arty {

Result System::process(Ptr<Blackboard> const&) { return ok(); }

Result System::process(Ptr<Blackboard> const& board,
                       Ptr<Keyboard> const& /*keyboard*/) {
  return process(board);
}

Result System::init(Ptr<Blackboard> const&) { return ok(); }

Result System::init(const Ptr<Blackboard>& board,
                    const Ptr<Keyboard>& /*keyboard*/) {
  return init(board);
}

void System::release() {}

}  // namespace arty
