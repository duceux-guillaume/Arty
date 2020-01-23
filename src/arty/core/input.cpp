#include <arty/core/input.hpp>

namespace arty {

int InputEvent::_count = 0;

void Keyboard::process(const Keyboard::Key &key,
                       const Keyboard::Action &action) {
  std::cout << "Key: " << key << " " << action << std::endl;
  auto const &submapping = mapping_[key];
  auto it = submapping.find(action);
  if (it != submapping.end()) {
    _incomings.insert(it->second);
  } else {
    std::cout << "no event registered" << std::endl;
  }
}

bool Keyboard::registerKeyEvent(const Keyboard::Key &key,
                                const Keyboard::Action &action,
                                const Keyboard::event_t &event) {
  std::cout << "event registered: " << key << " " << action << " " << event
            << std::endl;
  auto &submapping = mapping_[key];
  return submapping.insert(std::make_pair(action, event)).second;
}

Keyboard::event_t Keyboard::generate(const std::string &name) {
  return event_t(name);
}

void Keyboard::flush() {
  std::swap(_pool, _incomings);
  _incomings.clear();
}

bool Keyboard::occured(const Keyboard::event_t &e) {
  return _pool.find(e) != _pool.end();
}

}  // namespace arty

std::ostream &std::operator<<(std::ostream &out,
                              const arty::InputEvent &event) {
  out << event.name() << event.uuid();
  return out;
}
