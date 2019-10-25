#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GL/glew.h>
#include <arty/core/ecs.h>
#include <arty/core/math.h>

namespace arty {

struct Mesh {
  GLuint vbo;
  std::vector<GLfloat> buffer;

  Mesh() : vbo(0), buffer() {}
  Mesh(std::vector<GLfloat>&& vector) : vbo(0), buffer(vector) {}
};

class OpenGlRenderer : public System {
 public:
  OpenGlRenderer();
  ~OpenGlRenderer();

  Result init(Ptr<Blackboard> const& board) override;
  Result process(Ptr<Blackboard> const& board) override;
  void release() override;

 private:
  GLuint _program_id;
  GLuint _vertexbuffer;
  GLuint _uvbuffer;
  GLuint _vertexarrayid;
  GLuint MatrixID;
  GLuint TextureID;
  GLuint Texture;
  std::vector<Vec3f> vertices;
  std::vector<Vec2f> uvs;
  std::vector<Vec3f> normals;
};

}  // namespace arty

#endif  // OPENGL_RENDERER_H
