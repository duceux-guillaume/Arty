#include <arty/core/system.hpp>

namespace arty {

Result System::process(Ptr<Blackboard> const &) { return ok(); }

Result System::init(Ptr<Blackboard> const &) { return ok(); }

void System::release() {}

}  // namespace arty
