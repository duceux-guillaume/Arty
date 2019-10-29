#ifndef MESH_HPP
#define MESH_HPP

#include <arty/core/math.h>

#include <string>
#include <vector>

namespace arty {

struct Mesh {
  std::string model_file;
  std::string texture_file;

  // TODO: remove this which is specifically for OpenGl
  uint32_t vertices_vbo;
  uint32_t uvs_vbo;
  uint32_t normals_vbo;
  uint32_t indices_vbo;
  uint32_t colors_vbo;

  std::vector<Vec3f> vertices;
  std::vector<Vec2f> uvs;
  std::vector<Vec3f> normals;
  std::vector<Vec4f> colors;
  std::vector<uint16_t> indices;

  bool hasTexture() const { return uvs.size() > 0 && texture_file.size() > 0; }
  bool hasNormals() const { return normals.size() > 0; }
  bool hasColors() const { return colors.size() > 0; }
};

}  // namespace arty

#endif  // MESH_HPP
