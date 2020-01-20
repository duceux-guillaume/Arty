#ifndef MESH_LOADER_SYSTEM_HPP
#define MESH_LOADER_SYSTEM_HPP

#include <arty/core/result.h>

#include <arty/core/mesh.hpp>
#include <arty/core/system.hpp>

namespace arty {

class Loader {
 public:
  Result loadObj(std::string const& path, Mesh* out);
  Result loadMtl(std::string const& path, Material* out);

 private:
};

class MeshLoaderSystem : public System {
 public:
 private:
  Loader loader;

  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
  Result init(const Ptr<Memory>& board) override;
  void release() override;
};

}  // namespace arty

#endif  // MESH_LOADER_SYSTEM_HPP
