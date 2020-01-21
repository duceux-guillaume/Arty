#ifndef GL_LOADER_H
#define GL_LOADER_H

#include <arty/core/math.hpp>
#include <arty/core/mesh.hpp>
#include <arty/core/result.hpp>
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

}  // namespace arty

#endif  // GL_LOADER_H
