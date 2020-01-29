#include <arty/core/system.hpp>

namespace arty {

Result System::process(Ptr<Memory> const&) { return ok(); }

Result System::process(Ptr<Memory> const& board,
                       Ptr<InputManager> const& /*keyboard*/) {
  return process(board);
}

Result System::init(Ptr<Memory> const&) { return ok(); }

Result System::init(Ptr<Memory> const& board,
                    Ptr<InputManager> const& /*keyboard*/) {
  return init(board);
}

void System::release() {}

Result EventSystem::process(const Ptr<Memory>& board,
                            const Ptr<InputManager>& inputs) {
  if (inputs->pop(_event)) {
    return _job(board);
  }
  return ok();
}

Result EventSystem::init(const Ptr<Memory>& /*board*/,
                         const Ptr<InputManager>& inputs) {
  if (!inputs->attach(_input, _event)) {
    return error("input already taken: " + std::to_string(_input.key()));
  }
  return ok();
}

}  // namespace arty
