#ifndef WINDOW_H
#define WINDOW_H

#include <arty/core/system.hpp>

namespace arty {

struct CursorPosition {
  double x;
  double y;

  CursorPosition() : x(0.0), y(0.0) {}
  CursorPosition(double x, double y) : x(x), y(y) {}
};

class Window {
 public:
  virtual Result init() = 0;
  virtual void clear() = 0;
  virtual void swapBuffer() = 0;
  virtual bool isOk() = 0;
  virtual void close() = 0;
  virtual CursorPosition getCursorPosition() = 0;
  virtual void setCursorPosition(CursorPosition const& cursor) = 0;
  virtual double getTime() = 0;
  virtual int width() = 0;
  virtual int height() = 0;
};
}  // namespace arty

#endif  // WINDOW_H
