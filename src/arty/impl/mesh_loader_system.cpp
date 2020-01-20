#include <arty/impl/mesh_loader_system.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace arty {

Result Loader::loadObj(const std::string &path, Mesh *out) {
  if (!out) {
    return error("given empty pointer");
  }
  if (path.size() == 0) {
    return error("given empty path");
  }
  Mesh tmp;
  std::ifstream file;
  file.open(path);
  if (!file.is_open()) {
    return error("error opening file: " + path);
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    // remove comments
    if (line[0] == '#') {
      continue;
    }
    std::istringstream ss(line);
    std::string header;
    ss >> header;
    if (header.compare("v") == 0) {
      float x, y, z;
      ss >> x >> y >> z;
      tmp.vertices.emplace_back(Vec3f{x, y, z});
    } else if (header.compare("vt") == 0) {
      float u, v;
      ss >> u >> v;
      tmp.uvs.emplace_back(Vec2f{u, v});
    } else if (header.compare("vn") == 0) {
      float x, y, z;
      ss >> x >> y >> z;
      tmp.normals.emplace_back(Vec3f{x, y, z});
    } else if (header.compare("f") == 0) {
      std::size_t count = 0;
      std::string element;
      while (std::getline(ss, element, ' ')) {
        if (element.size() == 0) {
          continue;
        }
        std::istringstream index(element);
        std::string sub;
        while (std::getline(index, sub, '/')) {
          count++;
          uint16_t ind;
          std::istringstream convert(sub);
          convert >> ind;
          tmp.indices.push_back(ind);
        }
      }
      if (count / 3 == 4) {
        tmp.type = FaceType::QUAD;
        //  We need to produce triangles, cause I can't render them
      } else {
        tmp.type = FaceType::TRIANGLE;
      }
    } else {
      //  std::cout << "unhandled: " << header << std::endl;
    }
  }
  // std::cout << "loaded! vertex: " << tmp.vertices.size()
  //          << " normals: " << tmp.normals.size() << " uvs: " <<
  //          tmp.uvs.size()
  //          << " indices: " << tmp.indices.size() << std::endl;
  // std::cout << "start optimizing mesh" << std::endl;

  out->type = TRIANGLE;
  if (tmp.type == QUAD) {
    // std::cout << "converting quads to triangles" << std::endl;
    // We need to reorganized again to transform quads into triangles
    auto cpy = tmp.indices;
    out->indices.clear();
    for (std::size_t i = 0; i < cpy.size() - 3; i += 4) {
      tmp.indices.push_back(cpy[i]);
      tmp.indices.push_back(cpy[i + 1]);
      tmp.indices.push_back(cpy[i + 2]);
      tmp.indices.push_back(cpy[i]);
      tmp.indices.push_back(cpy[i + 2]);
      tmp.indices.push_back(cpy[i + 3]);
    }
  }

  for (std::size_t i = 0; i < tmp.indices.size() - 2; i += 3) {
    uint16_t v, vt, vn;
    v = tmp.indices[i];
    vt = tmp.indices[i + 1];
    vn = tmp.indices[i + 2];
    // std::cout << "f " << v << "/" << vt << "/" << vn << std::endl;

    uint16_t id = out->vertices.size() + 1;
    out->vertices.push_back(tmp.vertices[v - 1]);
    out->uvs.push_back(tmp.uvs[vt - 1]);
    out->normals.push_back(tmp.normals[vn - 1]);
    out->indices.push_back(id);
  }
  return ok();
}

Result MeshLoaderSystem::process(const Ptr<Memory> &board) {
  auto ptr = board->getProperties<std::string>("model2load");
  if (!ptr) {
    return ok();
  }
  for (auto const &prop : *ptr) {
    Mesh mesh;
    check_result(loader.loadObj(prop.value, &mesh));
    board->set(prop.entity, "mesh2import", mesh);
  }
  board->clearProperties("model2load");
  return ok();
}

Result MeshLoaderSystem::init(const Ptr<Memory> & /*board*/) {
  return ok();
}

void MeshLoaderSystem::release() {}

}  // namespace arty
