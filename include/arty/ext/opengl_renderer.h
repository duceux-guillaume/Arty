#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GL/glew.h>
#include <arty/core/math.h>

#include <arty/core/mesh.hpp>
#include <arty/core/system.hpp>

namespace arty {

struct OpenGlPtrs {
  GLuint array;
  GLuint program;
  GLuint mvp;
};

struct MeshVbos {
  GLuint vertex;
  GLuint element;
  GLuint normal;
};

class Renderer {
 public:
  Result init();
  void release();

  void importDynMeshNoTextureNoIndices(Mesh const& m);
  Result drawDynMeshNoTextureNoIndices(const Mesh& m, const Mat4x4f& mvp);

  void importStaticMeshNoTextureNoIndices(Mesh const& m);
  Result drawStaticMeshNoTextureNoIndices(const Mesh& m, const Mat4x4f& mvp);

  void importStaticMeshNoTexture(Mesh const& m);
  Result drawStaticMeshNoTexture(const Mesh& mesh, const Mat4x4f& model,
                                 const Mat4x4f& view, const Mat4x4f& proj);

 private:
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
