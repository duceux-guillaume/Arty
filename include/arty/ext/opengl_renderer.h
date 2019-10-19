#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GL/glew.h>
#include <arty/core/ecs.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

namespace arty {

struct Mesh {
  GLuint vbo;
  std::vector<GLfloat> buffer;
};

class OpenGlRenderer : public System {
 public:
  OpenGlRenderer();
  ~OpenGlRenderer();

  Result init(Ptr<Blackboard> const& board) override;
  Result process(Ptr<Blackboard> const& board) override;
  void release() override;

  GLuint LoadShaders(const char* vertex_file_path,
                     const char* fragment_file_path);

 private:
  GLuint _program_id;
  GLuint _vertexbuffer;
  GLuint _vertexarrayid;
  GLuint MatrixID;
  glm::mat4 MVP;
};

}  // namespace arty

#endif  // OPENGL_RENDERER_H
