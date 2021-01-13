#ifndef ARTY_CORE_WINDOW_HPP
#define ARTY_CORE_WINDOW_HPP

#include <arty/core/system.hpp>

namespace arty {

class WindowMode {
 public:
  enum Type { WINDOWED, FULLSCREEN };

  static WindowMode FullScreen();
  static WindowMode Windowed(int w, int h);

  Type type() const { return _type; }
  int width() const { return _width; }
  int height() const { return _height; }

 private:
  Type _type;
  int _width;
  int _height;
};

class Window {
 protected:
  WindowMode _mode;
  std::string _name;

 public:
  Window(WindowMode mode, std::string const name) : _mode(mode), _name(name) {}

  virtual ~Window() {}

  virtual Result init() = 0;
  virtual void clear() = 0;
  virtual void refresh() = 0;
  virtual bool isOk() const = 0;
  virtual void close() = 0;
  virtual double getTime() = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual bool isVisible() const = 0;
};
}  // namespace arty

#endif  // ARTY_CORE_WINDOW_HPP
