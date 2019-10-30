#ifndef MESH_LOADER_SYSTEM_HPP
#define MESH_LOADER_SYSTEM_HPP

#include <arty/core/result.h>

#include <arty/core/mesh.hpp>

namespace arty {

class Loader {
 public:
  Result loadObj(std::string const& path, Mesh* out);
  Result loadMtl(std::string const& path, Material* out);

 private:
};

}  // namespace arty

#endif  // MESH_LOADER_SYSTEM_HPP
