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

  void importStaticMeshNoTexture(Mesh const& m, MeshVbos& out);
  Result drawStaticMeshNoTexture(const Mesh& mesh, const Mat4x4f& model,
                                 const Mat4x4f& view, const Mat4x4f& proj,
                                 const MeshVbos& vbos);

  void importStreamMeshNoTexture(const Mesh& m, MeshVbos& out);

  Result drawStreamMeshNoTexture(const Mesh& mesh, const Mat4x4f& model,
                                 const Mat4x4f& view, const Mat4x4f& proj,
                                 const MeshVbos& vbos);

 private:
};

class OpenGlRenderer : public System {
 public:
  OpenGlRenderer();
  ~OpenGlRenderer();

  Result init(Ptr<Memory> const& board) override;
  Result process(Ptr<Memory> const& board) override;
  void release() override;

 private:
  Renderer _renderer;
};

}  // namespace arty

#endif  // OPENGL_RENDERER_H
