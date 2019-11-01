#ifndef TWOD_RENDERER_HPP
#define TWOD_RENDERER_HPP

#include <arty/core/renderer.hpp>

namespace arty {

class GlTextRenderer : public ITextRenderer {
 public:
  Result initText2D(const char* texturePath);
  void printText2D(const char* text, int x, int y, int size);

 private:
  unsigned int _text2DTextureID;
  unsigned int _text2DVertexBufferID;
  unsigned int _text2DUVBufferID;
  unsigned int _text2DShaderID;
  unsigned int _text2DUniformID;

  // ITextRenderer interface
 public:
  Result initText2D(const std::string& texturePath) override {
    return initText2D(texturePath.c_str());
  }
  void printText2D(const std::string& text, int x, int y, int size) override {
    printText2D(text.c_str(), x, y, size);
  }
  void cleanupText2D() override;
};

}  // namespace arty

#endif  // 2D_RENDERER_HPP
