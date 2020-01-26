#include <arty/core/input.hpp>

namespace arty {

int InputEvent::_count = 0;

void InputDevice::process(int key, Action const& action) {
  auto const& submapping = mapping_[key];
  auto it = submapping.find(action);
  if (it != submapping.end()) {
    _incomings.insert(it->second);
  }
}

bool InputDevice::registerEvent(int key, Action const& action,
                                event_t const& event) {
  auto& submapping = mapping_[key];
  return submapping.insert(std::make_pair(action, event)).second;
}

InputDevice::event_t InputDevice::generate(const std::string& name) {
  return event_t(name);
}

void InputDevice::flush() {
  std::swap(_pool, _incomings);
  _incomings.clear();
}

bool InputDevice::occured(InputDevice::event_t const& e) {
  return _pool.find(e) != _pool.end();
}

}  // namespace arty

std::ostream& std::operator<<(std::ostream& out,
                              const arty::InputEvent& event) {
  out << event.name() << event.uuid();
  return out;
}
