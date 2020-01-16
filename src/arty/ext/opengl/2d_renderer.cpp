#include <GL/glew.h>
#include <arty/core/math.h>
#include <arty/ext/opengl/gl_loader.h>

#include <arty/ext/opengl/2d_renderer.hpp>

namespace arty {

Result GlTextRenderer::initText2D(const char *texturePath) {
  glGenVertexArrays(1, &_test2DVertexArrayID);
  glBindVertexArray(_test2DVertexArrayID);

  // Initialize text'ure
  _text2DTextureID = loadDDS(texturePath);
  if (_text2DTextureID == 0) {
    return error(std::string("failed to load texture ") +
                 std::string(texturePath));
  }

  // Initialize VBO
  glGenBuffers(1, &_text2DVertexBufferID);
  glGenBuffers(1, &_text2DUVBufferID);

  // Initialize Shader
  _text2DShaderID = LoadShaders("../shaders/TextVertexShader.vertexshader",
                                "../shaders/TextVertexShader.fragmentshader");
  if (_text2DShaderID == 0) {
    return error(std::string("failed to load shader ") +
                 std::string(texturePath));
  }
  // Initialize uniforms' IDs
  _text2DUniformID = glGetUniformLocation(_text2DShaderID, "myTextureSampler");
  return ok();
}

void GlTextRenderer::printText2D(const char *text, int x, int y, int size) {
  unsigned int length = std::strlen(text);

  // Fill buffers
  std::vector<Vec2f> vertices;
  std::vector<Vec2f> UVs;
  for (unsigned int i = 0; i < length; i++) {
    float posx = x + i * size;
    float posy = y;
    Vec2f vertex_up_left = Vec2f(posx, posy + size);
    Vec2f vertex_up_right = Vec2f(posx + size, posy + size);
    Vec2f vertex_down_right = Vec2f(posx + size, posy);
    Vec2f vertex_down_left = Vec2f(posx, posy);

    vertices.push_back(vertex_up_left);
    vertices.push_back(vertex_down_left);
    vertices.push_back(vertex_up_right);

    vertices.push_back(vertex_down_right);
    vertices.push_back(vertex_up_right);
    vertices.push_back(vertex_down_left);

    char character = text[i];
    float uv_x = (character % 16) / 16.0f;
    float uv_y = (character / 16) / 16.0f;

    Vec2f uv_up_left = Vec2f(uv_x, uv_y);
    Vec2f uv_up_right = Vec2f(uv_x + 1.0f / 16.0f, uv_y);
    Vec2f uv_down_right = Vec2f(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
    Vec2f uv_down_left = Vec2f(uv_x, (uv_y + 1.0f / 16.0f));
    UVs.push_back(uv_up_left);
    UVs.push_back(uv_down_left);
    UVs.push_back(uv_up_right);

    UVs.push_back(uv_down_right);
    UVs.push_back(uv_up_right);
    UVs.push_back(uv_down_left);
  }
  glBindBuffer(GL_ARRAY_BUFFER, _text2DVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec2f), &vertices[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, _text2DUVBufferID);
  glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(Vec2f), &UVs[0],
               GL_STATIC_DRAW);

  // Bind shader
  glUseProgram(_text2DShaderID);

  // Bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _text2DTextureID);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(_text2DUniformID, 0);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _text2DVertexBufferID);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, _text2DUVBufferID);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Draw call
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void GlTextRenderer::cleanupText2D() {
  // Delete buffers
  glDeleteBuffers(1, &_text2DVertexBufferID);
  glDeleteBuffers(1, &_text2DUVBufferID);

  // Delete texture
  glDeleteTextures(1, &_text2DTextureID);

  // Delete shader
  glDeleteProgram(_text2DShaderID);
}

}  // namespace arty
