#ifndef TWOD_RENDERER_HPP
#define TWOD_RENDERER_HPP

#include <arty/core/memory.hpp>
#include <arty/core/renderer.hpp>
#include <unordered_map>

namespace arty {

class GlRenderer2D : public ITextRenderer {
 public:
  Result initText2D(const char* texturePath);
  void drawText2D(const char* text, int x, int y, int size);
  void releaseText2D();

 private:
  unsigned int _text2DVertexArrayID;
  unsigned int _text2DTextureID;
  unsigned int _text2DVertexBufferID;
  unsigned int _text2DUVBufferID;
  unsigned int _text2DShaderID;
  int _text2DUniformID;

  GLuint _shaderProgram;
  GLuint _fragmentShader;
  GLuint _vertexShader;
  GLuint _ebo;
  GLuint _vbo;
  GLuint _vao;
  GLint _posAttrib;
  GLint _colAttrib;

  // IRenderer2D interface
 public:
  Result loadTextTexture(const std::string& texturePath) override {
    return initText2D(texturePath.c_str());
  }

  void draw(Text const& text) override {
    drawText2D(text.content.c_str(), text.position.x(), text.position.y(),
               text.size);
  }

  void release() override { releaseText2D(); }
};

}  // namespace arty

#endif  // 2D_RENDERER_HPP
