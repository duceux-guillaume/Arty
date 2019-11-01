#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <arty/core/result.h>

#include <string>

namespace arty {

class ITextRenderer {
 public:
  virtual Result initText2D(std::string const& texturePath) = 0;
  virtual void printText2D(std::string const& text, int x, int y, int size) = 0;
  virtual void cleanupText2D() = 0;
};

}  // namespace arty

#endif  // RENDERER_HPP
