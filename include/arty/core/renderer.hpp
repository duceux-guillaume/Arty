#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <arty/core/gui/rectangle.hpp>
#include <arty/core/gui/text.hpp>
#include <arty/core/result.hpp>
#include <string>

namespace arty {

class ITextRenderer {
 public:
  virtual Result loadTextTexture(std::string const& texturePath) = 0;
  virtual void draw(Text const& text) = 0;
  virtual void release() = 0;
};

class Renderer {
 private:
  Ptr<ITextRenderer> _text_renderer;

 public:
  Renderer(Ptr<ITextRenderer> text) : _text_renderer(text) {}

  Result loadTextTexture(std::string const& file) {
    return _text_renderer->loadTextTexture(file);
  }

  void release() { _text_renderer->release(); }

  void draw(Text const& text) { _text_renderer->draw(text); }
  void draw(Rectangle const& /*rect*/) { /*_2d_renderer->draw(rect);*/
  }
};

}  // namespace arty

#endif  // RENDERER_HPP
