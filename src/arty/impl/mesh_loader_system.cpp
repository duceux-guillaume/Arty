#include <arty/impl/mesh_loader_system.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace arty {

Result Loader::load(const std::string &path, Mesh *out) {
  if (!out) {
    return error("given empty pointer");
  }
  if (path.size() == 0) {
    return error("given empty path");
  }
  std::cout << "loading mesh: " << path << std::endl;
  std::ifstream file;
  file.open(path);
  if (!file.is_open()) {
    return error("error opening file: " + path);
  }

  std::string line;
  while (std::getline(file, line)) {
    std::cout << line << std::endl;
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
      out->vertices.emplace_back(Vec3f{x, y, z});
    } else if (header.compare("vt") == 0) {
      float u, v;
      ss >> u >> v;
      out->uvs.emplace_back(Vec2f{u, v});
    } else if (header.compare("vn") == 0) {
      float x, y, z;
      ss >> x >> y >> z;
      out->normals.emplace_back(Vec3f{x, y, z});
    } else if (header.compare("f") == 0) {
      std::size_t count = 0;
      std::string element;
      std::vector<uint16_t> tmp;
      while (std::getline(ss, element, ' ')) {
        if (element.size() == 0) {
          continue;
        }
        std::istringstream index(element);
        std::string sub;
        while (std::getline(index, sub, '/')) {
          count++;
          if (count % 3 == 1) {
            uint16_t ind;
            std::istringstream convert(sub);
            convert >> ind;
            tmp.push_back(ind);
          }
        }
      }
      if (count / 3 == 4) {
        out->type = FaceType::QUAD;
        //  We need to produce triangles, cause I can't render them
        out->indices.push_back(tmp[0]);
        out->indices.push_back(tmp[1]);
        out->indices.push_back(tmp[2]);
        out->indices.push_back(tmp[0]);
        out->indices.push_back(tmp[2]);
        out->indices.push_back(tmp[3]);
      } else {
        out->type = FaceType::TRIANGLE;
      }
    } else {
      std::cout << "unhandled: " << header << std::endl;
    }
  }
  std::cout << "loaded! vertex: " << out->vertices.size()
            << " normals: " << out->normals.size() << std::endl;

  return ok();
}

}  // namespace arty
