#include <arty/ext/opengl_renderer.h>

#include <cstring>
#include <fstream>
#include <sstream>

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

namespace arty {
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

  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit
  // <-> 100 units
  glm::mat4 Projection =
      glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // Or, for an ortho camera :
  // glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f,
  // 100.0f);
  // // In world coordinates

  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(4, 3, 3),  // Camera is at (4,3,3), in World Space
      glm::vec3(0, 0, 0),  // and looks at the origin
      glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
  );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = Projection * View *
        Model;  // Remember, matrix multiplication is the other way around

  // Load the texture using any two methods
  Texture = loadBMP_custom("texture/uvtemplate.bmp");
  // Texture = loadDDS("texture/uvtemplate.DDS");
  if (Texture == 0) {
    return error("error loading texture");
  }

  // Get a handle for our "myTextureSampler" uniform
  TextureID = glGetUniformLocation(_program_id, "myTextureSampler");

  auto ptrit1 = board->get_property<Mesh>("mesh");
  for (auto& mesh : *ptrit1) {
    glGenBuffers(1, &mesh.val().vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.val().vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.val().buffer.size() * sizeof(GLfloat),
                 mesh.val().buffer.data(), GL_STATIC_DRAW);
  }
  auto ptrit2 = board->get_property<Mesh>("texture");
  for (auto& mesh : *ptrit2) {
    glGenBuffers(1, &mesh.val().vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.val().vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.val().buffer.size() * sizeof(GLfloat),
                 mesh.val().buffer.data(), GL_STATIC_DRAW);
  }

  return ok();
}

Result OpenGlRenderer::process(const Ptr<Blackboard>& board) {
  // Use our shader
  glUseProgram(_program_id);

  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(TextureID, 0);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  {
    auto ptrit = board->get_property<Mesh>("mesh");
    for (auto& mesh : *ptrit) {
      glBindBuffer(GL_ARRAY_BUFFER, mesh.val().vbo);
      glVertexAttribPointer(0,  // attribute 0. No particular reason for 0, but
                                // must match the layout in the shader.
                            3,  // size
                            GL_FLOAT,  // type
                            GL_FALSE,  // normalized?
                            0,         // stride
                            (void*)0   // array buffer offset
      );
    }
  }

  glEnableVertexAttribArray(1);
  {
    auto ptrit = board->get_property<Mesh>("texture");
    for (auto& mesh : *ptrit) {
      glBindBuffer(GL_ARRAY_BUFFER, mesh.val().vbo);
      glVertexAttribPointer(1,  // attribute 0. No particular reason for 0, but
                                // must match the layout in the shader.
                            2,  // size
                            GL_FLOAT,  // type
                            GL_FALSE,  // normalized?
                            0,         // stride
                            (void*)0   // array buffer offset
      );
    }
  }

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0,
               3 * 12);  // 3 indices starting at 0 -> 1 triangle

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
