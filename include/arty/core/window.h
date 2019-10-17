#ifndef WINDOW_H
#define WINDOW_H

#include <arty/core/ecs.h>

namespace arty {
class Window {
 public:
  virtual Result init() = 0;
  virtual void clear() = 0;
  virtual void swapBuffer() = 0;
  virtual bool isOk() = 0;
  virtual void close() = 0;
};
}  // namespace arty

#endif  // WINDOW_H
