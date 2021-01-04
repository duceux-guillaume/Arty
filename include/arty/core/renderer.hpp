#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <arty/core/gui/text.hpp>
#include <arty/core/result.hpp>
#include <string>

namespace arty {

class ITextRenderer {
 public:
  virtual Result initText2D(std::string const& texturePath) = 0;
  virtual void printText2D(std::string const& text, int x, int y, int size) = 0;
  virtual void cleanupText2D() = 0;
};

class Renderer {
 private:
  Ptr<ITextRenderer> _text_renderer;

 public:
  Renderer(Ptr<ITextRenderer> text) : _text_renderer(text) {}

  Result loadFont(std::string const& file) {
    return _text_renderer->initText2D(file);
  }

  void release() { _text_renderer->cleanupText2D(); }

  void draw(Text const& text) {
    _text_renderer->printText2D(text.content, text.position.x(),
                                text.position.y(), text.size);
  }
};

}  // namespace arty

#endif  // RENDERER_HPP
