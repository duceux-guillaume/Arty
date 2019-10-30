#ifndef MESH_LOADER_SYSTEM_HPP
#define MESH_LOADER_SYSTEM_HPP

#include <arty/core/result.h>
#include <arty/core/mesh.hpp>

namespace arty {

class Loader {
public:
    Result load(std::string const& path, Mesh* out);

private:
};

}  // namespace arty

#endif // MESH_LOADER_SYSTEM_HPP
