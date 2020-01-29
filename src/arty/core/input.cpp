#include <arty/core/input.hpp>

namespace arty {

int Event::_count = 0;

void Device::process(int key, Action const& action) {
  auto const& submapping = mapping_[key];
  auto it = submapping.find(action);
  if (it != submapping.end()) {
    _incomings.insert(it->second);
  }
}

bool Device::registerEvent(int key, Action const& action,
                           event_t const& event) {
  auto& submapping = mapping_[key];
  return submapping.insert(std::make_pair(action, event)).second;
}

Device::event_t Device::generate(const std::string& name) {
  return event_t(name);
}

void Device::flush() {
  std::swap(_pool, _incomings);
  _incomings.clear();
}

bool Device::occured(Device::event_t const& e) {
  return _pool.find(e) != _pool.end();
}

bool InputManager::attach(const Input& in, const Event& ev) {
  if (in.type() == Input::Type::Keyboard) {
    return attach(in.key(), in.action(), ev);
  }
  if (in.type() == Input::Type::Mouse) {
    return attach(in.button(), in.action(), ev);
  }
  return false;
}

bool InputManager::attach(const Keyboard::Key& key,
                          const Device::Action& action, const Event& event) {
  if (!_keyboard) {
    return false;
  }
  return _keyboard->registerKeyEvent(key, action, event);
}

bool InputManager::attach(const Mouse::Button& key,
                          const Device::Action& action, const Event& event) {
  if (!_mouse) {
    return false;
  }
  return _mouse->registerEvent(key, action, event);
}

bool InputManager::pop(const Event& event) {
  if (!_mouse && !_keyboard) {
    return false;
  }
  if (!_mouse) {
    return _keyboard->occured(event);
  }
  if (!_keyboard) {
    return _mouse->occured(event);
  }
  return _mouse->occured(event) || _keyboard->occured(event);
}

Mouse::position_type InputManager::position() const {
  if (!_mouse) {
    return Mouse::position_type();
  }
  return _mouse->position();
}

void InputManager::set(const Mouse::position_type& pos) {
  if (_mouse) {
    _mouse->set(pos);
  }
}

void InputManager::flush() {
  if (_mouse) {
    _mouse->flush();
  }
  if (_keyboard) {
    _keyboard->flush();
  }
}
}  // namespace arty

std::ostream& std::operator<<(std::ostream& out, const arty::Event& event) {
  out << event.name() << event.uuid();
  return out;
}
