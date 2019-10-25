#include <arty/core/math.h>
#include <arty/ext/opengl_renderer.h>

namespace arty {

OpenGlRenderer::OpenGlRenderer()
    : _program_id(0), _vertexbuffer(0), _vertexarrayid(0) {}

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

}  // namespace arty
