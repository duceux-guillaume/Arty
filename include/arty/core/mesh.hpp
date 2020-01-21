#ifndef MESH_HPP
#define MESH_HPP

#include <arty/core/math.hpp>
#include <string>
#include <vector>

namespace arty {

// OpenGl doesn't want Quads ...
// Those are converted at load time
enum FaceType {
  QUAD,
  TRIANGLE,
};

struct Mesh {
  std::vector<Vec3f> vertices;
  std::vector<Vec2f> uvs;
  std::vector<Vec3f> normals;
  std::vector<Vec4f> colors;
  std::vector<uint16_t> indices;
  FaceType type;

  bool hasTexture() const { return uvs.size() > 0; }
  bool hasNormals() const { return normals.size() > 0; }
  bool hasColors() const { return colors.size() > 0; }
};

struct Material {};

}  // namespace arty

#endif  // MESH_HPP
