#include <arty/core/math.h>
#include <arty/ext/opengl_renderer.h>

#include <cstring>
#include <fstream>
#include <sstream>

namespace arty {

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

static bool loadOBJ(const char* path, std::vector<Vec3f>& out_vertices,
                    std::vector<Vec2f>& out_uvs,
                    std::vector<Vec3f>& out_normals) {
  printf("Loading OBJ file %s...\n", path);

  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<Vec3f> temp_vertices;
  std::vector<Vec2f> temp_uvs;
  std::vector<Vec3f> temp_normals;

  FILE* file = fopen(path, "r");
  if (file == NULL) {
    printf(
        "Impossible to open the file ! Are you in the right path ? See "
        "Tutorial 1 for details\n");
    getchar();
    return false;
  }

  while (1) {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF) break;  // EOF = End Of File. Quit the loop.

    // else : parse lineHeader

    if (strcmp(lineHeader, "v") == 0) {
      Vec3f vertex;
      fscanf(file, "%f %f %f\n", &vertex.x(), &vertex.y(), &vertex.z());
      temp_vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) {
      Vec2f uv;
      fscanf(file, "%f %f\n", &uv.x(), &uv.y());
      uv.y() = -uv.y();  // Invert V coordinate since we will only use DDS
                         // texture, which are inverted. Remove if you want to
                         // use TGA or BMP loaders.
      temp_uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      Vec3f normal;
      fscanf(file, "%f %f %f\n", &normal.x(), &normal.y(), &normal.z());
      temp_normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                           &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                           &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9) {
        printf(
            "File can't be read by our simple parser :-( Try exporting with "
            "other options\n");
        fclose(file);
        return false;
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    } else {
      // Probably a comment, eat up the rest of the line
      char stupidBuffer[1000];
      fgets(stupidBuffer, 1000, file);
    }
  }

  // For each vertex of each triangle
  for (unsigned int i = 0; i < vertexIndices.size(); i++) {
    // Get the indices of its attributes
    unsigned int vertexIndex = vertexIndices[i];
    unsigned int uvIndex = uvIndices[i];
    unsigned int normalIndex = normalIndices[i];

    // Get the attributes thanks to the index
    Vec3f vertex = temp_vertices[vertexIndex - 1];
    Vec2f uv = temp_uvs[uvIndex - 1];
    Vec3f normal = temp_normals[normalIndex - 1];

    // Put the attributes in buffers
    out_vertices.push_back(vertex);
    out_uvs.push_back(uv);
    out_normals.push_back(normal);
  }
  fclose(file);
  return true;
}

static GLuint loadBMP_custom(const char* imagepath) {
  printf("Reading image %s\n", imagepath);

  // Data read from the header of the BMP file
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int imageSize;
  unsigned int width, height;
  // Actual RGB data
  unsigned char* data;

  // Open the file
  FILE* file = fopen(imagepath, "rb");
  if (!file) {
    printf(
        "%s could not be opened. Are you in the right directory ? Don't forget "
        "to read the FAQ !\n",
        imagepath);
    getchar();
    return 0;
  }

  // Read the header, i.e. the 54 first bytes

  // If less than 54 bytes are read, problem
  if (fread(header, 1, 54, file) != 54) {
    printf("Not a correct BMP file\n");
    fclose(file);
    return 0;
  }
  // A BMP files always begins with "BM"
  if (header[0] != 'B' || header[1] != 'M') {
    printf("Not a correct BMP file\n");
    fclose(file);
    return 0;
  }
  // Make sure this is a 24bpp file
  if (*(int*)&(header[0x1E]) != 0) {
    printf("Not a correct BMP file\n");
    fclose(file);
    return 0;
  }
  if (*(int*)&(header[0x1C]) != 24) {
    printf("Not a correct BMP file\n");
    fclose(file);
    return 0;
  }

  // Read the information about the image
  dataPos = *(int*)&(header[0x0A]);
  imageSize = *(int*)&(header[0x22]);
  width = *(int*)&(header[0x12]);
  height = *(int*)&(header[0x16]);

  // Some BMP files are misformatted, guess missing information
  if (imageSize == 0)
    imageSize = width * height *
                3;  // 3 : one byte for each Red, Green and Blue component
  if (dataPos == 0) dataPos = 54;  // The BMP header is done that way

  // Create a buffer
  data = new unsigned char[imageSize];

  // Read the actual data from the file into the buffer
  fread(data, 1, imageSize, file);

  // Everything is in memory now, the file can be closed.
  fclose(file);

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
               GL_UNSIGNED_BYTE, data);

  // OpenGL has now copied the data. Free our own version
  delete[] data;

  // Poor filtering, or ...
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // ... nice trilinear filtering ...
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  // ... which requires mipmaps. Generate them automatically.
  glGenerateMipmap(GL_TEXTURE_2D);

  // Return the ID of the texture we just created
  return textureID;
}

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

static GLuint loadDDS(const char* imagepath) {
  unsigned char header[124];

  FILE* fp;

  /* try to open the file */
  fp = fopen(imagepath, "rb");
  if (fp == NULL) {
    printf(
        "%s could not be opened. Are you in the right directory ? Don't forget "
        "to read the FAQ !\n",
        imagepath);
    return 0;
  }

  /* verify the type of file */
  char filecode[4];
  fread(filecode, 1, 4, fp);
  if (strncmp(filecode, "DDS ", 4) != 0) {
    fclose(fp);
    return 0;
  }

  /* get the surface desc */
  fread(&header, 124, 1, fp);

  unsigned int height = *(unsigned int*)&(header[8]);
  unsigned int width = *(unsigned int*)&(header[12]);
  unsigned int linearSize = *(unsigned int*)&(header[16]);
  unsigned int mipMapCount = *(unsigned int*)&(header[24]);
  unsigned int fourCC = *(unsigned int*)&(header[80]);

  unsigned char* buffer;
  unsigned int bufsize;
  /* how big is it going to be including all mipmaps? */
  bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
  buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
  fread(buffer, 1, bufsize, fp);
  /* close the file pointer */
  fclose(fp);

  unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
  unsigned int format;
  switch (fourCC) {
    case FOURCC_DXT1:
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      break;
    case FOURCC_DXT3:
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      break;
    case FOURCC_DXT5:
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
    default:
      free(buffer);
      return 0;
  }

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glBindTexture(GL_TEXTURE_2D, textureID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  unsigned int blockSize =
      (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  unsigned int offset = 0;

  /* load the mipmaps */
  for (unsigned int level = 0; level < mipMapCount && (width || height);
       ++level) {
    unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size,
                           buffer + offset);

    offset += size;
    width /= 2;
    height /= 2;

    // Deal with Non-Power-Of-Two textures. This code is not included in the
    // webpage to reduce clutter.
    if (width < 1) width = 1;
    if (height < 1) height = 1;
  }

  free(buffer);

  return textureID;
}

OpenGlRenderer::OpenGlRenderer()
    : _program_id(0), _vertexbuffer(0), _vertexarrayid(0) {
  std::cout << "Hello" << std::endl;
}

OpenGlRenderer::~OpenGlRenderer() { std::cout << "ByeBye" << std::endl; }

Result OpenGlRenderer::init(Ptr<Blackboard> const& board) {
  glGenVertexArrays(1, &_vertexarrayid);
  glBindVertexArray(_vertexarrayid);

  // Create and compile our GLSL program from the shaders
  _program_id = LoadShaders("shaders/TransformVertexShader.vertexshader",
                            "shaders/TextureFragmentShader.fragmentshader");
  if (_program_id == 0) {
    return error("error loading shaders");
  }

  // Get a handle for our "MVP" uniform
  MatrixID = glGetUniformLocation(_program_id, "MVP");

  // Load the texture using any two methods
  // Texture = loadBMP_custom("texture/uvtemplate.bmp");
  // Texture = loadDDS("texture/uvtemplate.DDS");
  Texture = loadDDS("textures/uvmap.DDS");
  if (Texture == 0) {
    return error("error loading texture");
  }

  // Get a handle for our "myTextureSampler" uniform
  TextureID = glGetUniformLocation(_program_id, "myTextureSampler");

  // Read our .obj file
  bool res = loadOBJ("models/bot_1.obj", vertices, uvs, normals);
  if (!res) {
    return error("error loading obj");
  }

  glGenBuffers(1, &_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3f), &vertices[0],
               GL_STATIC_DRAW);

  glGenBuffers(1, &_uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vec2f), &uvs[0],
               GL_STATIC_DRAW);

  return ok();
}

Result OpenGlRenderer::process(const Ptr<Blackboard>& board) {
  // Get camera
  auto ptr = board->getProperty("mvp");
  assert(ptr);
  assert(ptr->size() == 1);
  Mat4x4f MVP = ptr->at(0).val();

  // Use our shader
  glUseProgram(_program_id);

  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, transpose(MVP).ptr());

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(TextureID, 0);

  // 1rst attribute buffer : vertices
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, _uvbuffer);
  glVertexAttribPointer(1,         // attribute
                        2,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0,
               vertices.size());  // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  return ok();
}  // namespace arty

void OpenGlRenderer::release() {
  glDeleteProgram(_program_id);
  // Cleanup VBO
  glDeleteBuffers(1, &_vertexbuffer);
  glDeleteVertexArrays(1, &_vertexarrayid);
}

GLuint OpenGlRenderer::LoadShaders(const char* vertex_file_path,
                                   const char* fragment_file_path) {
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::stringstream sstr;
    sstr << VertexShaderStream.rdbuf();
    VertexShaderCode = sstr.str();
    VertexShaderStream.close();
  } else {
    printf(
        "Impossible to open %s. Are you in the right directory ? Don't forget "
        "to read the FAQ !\n",
        vertex_file_path);
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if (FragmentShaderStream.is_open()) {
    std::stringstream sstr;
    sstr << FragmentShaderStream.rdbuf();
    FragmentShaderCode = sstr.str();
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const* VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
                       &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }

  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const* FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
                       &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }

  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
                        &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

}  // namespace arty
