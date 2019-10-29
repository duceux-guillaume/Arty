#ifndef GL_LOADER_H
#define GL_LOADER_H

#include <arty/core/math.h>
#include <arty/core/result.h>
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/scene.h>        // Output data structure

#include <arty/core/mesh.hpp>
#include <assimp/Importer.hpp>  // C++ importer interface
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

namespace arty {

unsigned int LoadShaders(const char* vertex_file_path,
                         const char* fragment_file_path);

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide :
// - Binary files. Reading a model should be just a few memcpy's away, not
// parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(const char* path, std::vector<Vec3f>& out_vertices,
             std::vector<Vec2f>& out_uvs, std::vector<Vec3f>& out_normals);

unsigned int loadBMP_custom(const char* imagepath);

// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another
// texture loading library, or do it yourself (just like loadBMP_custom and
// loadDDS)
// GLuint loadTGA_glfw(const char * imagepath){
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will
// modify this texture 	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Read the file, call glTexImage2D with the right parameters
//	glfwLoadTexture2D(imagepath, 0);
//
//	// Nice trilinear filtering.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
// GL_LINEAR_MIPMAP_LINEAR); 	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Return the ID of the texture we just created
//	return textureID;
//}

#define FOURCC_DXT1 0x31545844  // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844  // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844  // Equivalent to "DXT5" in ASCII

unsigned int loadDDS(const char* imagepath);

class Loader {
 public:
  template <typename MeshPtr>
  Result load(MeshPtr outmeshptr) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        outmeshptr->model_file,
        0 /*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
    if (!scene) {
      return error(importer.GetErrorString());
    }
    for (std::size_t i = 0; i < scene->mNumMeshes; ++i) {
      // We are going to put all file modeles in a single mesh
      const aiMesh* mesh = scene->mMeshes[i];

      // Fill vertices positions
      auto previous_size = outmeshptr->vertices.size();
      outmeshptr->vertices.reserve(previous_size + mesh->mNumVertices);
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D pos = mesh->mVertices[i];
        outmeshptr->vertices.push_back(Vec3f({pos.x, pos.y, pos.z}));
      }

      // Fill vertices texture coordinates
      if (mesh->HasTextureCoords(0)) {
        std::cout << outmeshptr->model_file << " has texture" << std::endl;
        previous_size = outmeshptr->uvs.size();
        outmeshptr->uvs.reserve(previous_size + mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
          aiVector3D UVW =
              mesh->mTextureCoords[0][i];  // Assume only 1 set of UV coords;
                                           // AssImp supports 8 UV sets.
          outmeshptr->uvs.push_back(Vec2f({UVW.x, UVW.y}));
        }
      }

      if (mesh->HasVertexColors(0)) {
        std::cout << outmeshptr->model_file << " has color" << std::endl;
        previous_size = outmeshptr->colors.size();
        outmeshptr->colors.reserve(previous_size + mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
          aiColor4D color =
              mesh->mColors[0][i];  // Assume only 1 set of UV coords;
                                    // AssImp supports 8 UV sets.
          outmeshptr->colors.push_back(
              Vec4f({color.r, color.g, color.b, color.a}));
        }
      }

      // Fill vertices normals
      previous_size = outmeshptr->normals.size();
      outmeshptr->normals.reserve(previous_size + mesh->mNumVertices);
      for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D n = mesh->mNormals[i];
        outmeshptr->normals.push_back(Vec3f({n.x, n.y, n.z}));
      }

      // Fill face indices
      previous_size = outmeshptr->indices.size();
      outmeshptr->indices.reserve(previous_size + 3 * mesh->mNumFaces);
      for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        // Assume the model has only triangles.
        outmeshptr->indices.push_back(mesh->mFaces[i].mIndices[0]);
        outmeshptr->indices.push_back(mesh->mFaces[i].mIndices[1]);
        outmeshptr->indices.push_back(mesh->mFaces[i].mIndices[2]);
      }
    }

    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
      std::cout << outmeshptr->model_file << " got materials" << std::endl;
      // TODO handle materials aka texture
      /*
      const aiMaterial* material = scene->mMaterials[i];
      m_Textures[i] = NULL;
      if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
          aiString Path;
          if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL,
      NULL, NULL, NULL) == AI_SUCCESS) { std::string FullPath = Dir + "/" +
      Path.data; m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

              if (!m_Textures[i]->Load()) {
                  printf("Error loading texture '%s'\n", FullPath.c_str());
                  delete m_Textures[i];
                  m_Textures[i] = NULL;
                  Ret = false;
              }
          }
          */
    }

    return ok();
  }

 private:
};  // namespace arty

}  // namespace arty

#endif  // GL_LOADER_H
