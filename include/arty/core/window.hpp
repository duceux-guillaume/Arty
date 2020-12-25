#ifndef WINDOW_H
#define WINDOW_H

#include <arty/core/system.hpp>

namespace arty {

class Window {
 public:
  virtual ~Window() {}

  virtual Result init() = 0;
  virtual void clear() = 0;
  virtual void swapBuffer() = 0;
  virtual bool isOk() const = 0;
  virtual void close() = 0;
  virtual double getTime() = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual bool isVisible() const = 0;
};
}  // namespace arty

#endif  // WINDOW_H
