#include <arty/core/math.h>
#include <arty/ext/gl_loader.h>
#include <arty/ext/opengl_renderer.h>

#include <arty/core/mesh.hpp>

namespace arty {

OpenGlRenderer::OpenGlRenderer()
    : _program_id(0), _vertexbuffer(0), _vertexarrayid(0) {}

OpenGlRenderer::~OpenGlRenderer() { std::cout << "ByeBye" << std::endl; }

Result OpenGlRenderer::init(Ptr<Blackboard> const& board) {
  glGenVertexArrays(1, &_vertexarrayid);
  glBindVertexArray(_vertexarrayid);

  auto shaderPtr = board->getProperties<Shader>("shader");
  auto meshPtr = board->getProperties<Mesh>("mesh");

  if (!shaderPtr || !meshPtr) {
    return error("nothing to render");
  }

  auto shaderIt = shaderPtr->buffer().begin();
  auto shaderEnd = shaderPtr->buffer().end();
  auto meshIt = meshPtr->buffer().begin();

  for (; shaderIt != shaderEnd; ++shaderIt, ++meshIt) {
    shaderIt->program =
        LoadShaders(shaderIt->vertex.c_str(), shaderIt->fragment.c_str());
    if (shaderIt->program == 0) {
      return error("error opening program file: " + shaderIt->vertex + " " +
                   shaderIt->fragment);
    }
    shaderIt->matrixId = glGetUniformLocation(shaderIt->program, "MVP");
    shaderIt->viewId = glGetUniformLocation(shaderIt->program, "V");
    shaderIt->modelId = glGetUniformLocation(shaderIt->program, "M");
    shaderIt->lightId =
        glGetUniformLocation(shaderIt->program, "LightPosition_worldspace");

    shaderIt->texture = loadDDS(shaderIt->textureFile.c_str());
    if (shaderIt->texture == 0) {
      return error("error loading texture");
    }
    shaderIt->textureId =
        glGetUniformLocation(shaderIt->program, "myTextureSampler");

    if (!loadOBJ(meshIt->model_file.c_str(), meshIt->vertices, meshIt->uvs,
                 meshIt->normals)) {
      return error("error loading obj file");
    }

    if (meshIt->hasTexture() && meshIt->hasNormals()) {
      glGenBuffers(1, &meshIt->vertices_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, meshIt->vertices_vbo);
      glBufferData(GL_ARRAY_BUFFER, meshIt->vertices.size() * sizeof(Vec3f),
                   NULL, GL_STREAM_DRAW);
      glGenBuffers(1, &meshIt->uvs_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, meshIt->uvs_vbo);
      glBufferData(GL_ARRAY_BUFFER, meshIt->uvs.size() * sizeof(Vec2f), NULL,
                   GL_STREAM_DRAW);
      glGenBuffers(1, &meshIt->normals_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, meshIt->normals_vbo);
      glBufferData(GL_ARRAY_BUFFER, meshIt->normals.size() * sizeof(Vec3f),
                   NULL, GL_STREAM_DRAW);
    }
  }
  return ok();
}

Result OpenGlRenderer::process(const Ptr<Blackboard>& board) {
  // Get camera
  Camera cam = board->getCamera();
  Mat4x4f VP = cam.projection * cam.view;

  auto shaderPtr = board->getProperties<Shader>("shader");
  auto meshPtr = board->getProperties<Mesh>("mesh");
  auto posPtr = board->getProperties<Transform>("transform");
  if (!shaderPtr || !meshPtr || !posPtr) {
    return error("nothing to render");
  }

  auto shaderIt = shaderPtr->buffer().begin();
  auto shaderEnd = shaderPtr->buffer().end();
  auto meshIt = meshPtr->buffer().begin();
  auto posIt = posPtr->buffer().begin();

  for (; shaderIt != shaderEnd; ++shaderIt, ++meshIt, ++posIt) {
    // Use our shader
    glUseProgram(shaderIt->program);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    Mat4x4f model = translation(posIt->position);
    Mat4x4f mvp = VP * model;
    glUniformMatrix4fv(shaderIt->matrixId, 1, GL_FALSE, transpose(mvp).ptr());
    glUniformMatrix4fv(shaderIt->modelId, 1, GL_FALSE, transpose(model).ptr());
    glUniformMatrix4fv(shaderIt->viewId, 1, GL_FALSE,
                       transpose(cam.view).ptr());

    Vec3f lightPos{4.f, 4.f, 4.f};
    glUniform3f(shaderIt->lightId, lightPos.x(), lightPos.y(), lightPos.z());

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shaderIt->texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(shaderIt->textureId, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, meshIt->vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, meshIt->vertices.size() * sizeof(Vec3f), NULL,
                 GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, meshIt->vertices.size() * sizeof(Vec3f),
                    meshIt->vertices.data());
    glVertexAttribPointer(0,         // attribute
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void*)0   // array buffer offset
    );

    if (meshIt->hasTexture()) {
      // 2nd attribute buffer : UVs
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, meshIt->uvs_vbo);
      glBufferData(GL_ARRAY_BUFFER, meshIt->uvs.size() * sizeof(Vec2f), NULL,
                   GL_STREAM_DRAW);
      glBufferSubData(GL_ARRAY_BUFFER, 0, meshIt->uvs.size() * sizeof(Vec2f),
                      meshIt->uvs.data());
      glVertexAttribPointer(1,         // attribute
                            2,         // size
                            GL_FLOAT,  // type
                            GL_FALSE,  // normalized?
                            0,         // stride
                            (void*)0   // array buffer offset
      );
    }

    if (meshIt->hasNormals()) {
      // 3rd attribute buffer : normals
      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, meshIt->normals_vbo);
      glBufferData(GL_ARRAY_BUFFER, meshIt->normals.size() * sizeof(Vec3f),
                   NULL, GL_STREAM_DRAW);
      glBufferSubData(GL_ARRAY_BUFFER, 0,
                      meshIt->normals.size() * sizeof(Vec3f),
                      meshIt->normals.data());
      glVertexAttribPointer(2,         // attribute
                            3,         // size
                            GL_FLOAT,  // type
                            GL_FALSE,  // normalized?
                            0,         // stride
                            (void*)0   // array buffer offset
      );
    }

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, meshIt->vertices.size());
    glDisableVertexAttribArray(0);
    if (meshIt->hasTexture()) {
      glDisableVertexAttribArray(1);
    }
    if (meshIt->hasNormals()) {
      glDisableVertexAttribArray(2);
    }
  }

  return ok();
}  // namespace arty

void OpenGlRenderer::release() {}

}  // namespace arty
