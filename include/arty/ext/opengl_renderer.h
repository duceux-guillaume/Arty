#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GL/glew.h>
#include <arty/core/ecs.h>

namespace arty {

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
};

}  // namespace arty

#endif  // OPENGL_RENDERER_H
