#include <arty/core/math.h>
#include <arty/ext/gl_loader.h>
#include <arty/ext/opengl_renderer.h>

namespace arty {

OpenGlRenderer::OpenGlRenderer()
    : _program_id(0), _vertexbuffer(0), _vertexarrayid(0) {}

OpenGlRenderer::~OpenGlRenderer() { std::cout << "ByeBye" << std::endl; }

Result OpenGlRenderer::init(Ptr<Blackboard> const& board) {
  glGenVertexArrays(1, &_vertexarrayid);
  glBindVertexArray(_vertexarrayid);

  auto shaderPtr = board->getProperties<Shader>("shader");
  auto vertexPtr = board->getProperties<Mesh>("mesh");

  if (!shaderPtr || !vertexPtr) {
    return error("nothing to render");
  }

  auto shaderIt = shaderPtr->buffer().begin();
  auto shaderEnd = shaderPtr->buffer().end();
  auto meshIt = vertexPtr->buffer().begin();

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

    if (!loadOBJ(vertexIt->file.c_str(), vertexIt->buffer, uvIt->buffer,
                 normalIt->buffer)) {
      return error("error loading obj file");
    }
    glGenBuffers(1, &vertexIt->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexIt->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexIt->buffer.size() * sizeof(Vec3f), NULL,
                 GL_STREAM_DRAW);
    glGenBuffers(1, &uvIt->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvIt->vbo);
    glBufferData(GL_ARRAY_BUFFER, uvIt->buffer.size() * sizeof(Vec2f), NULL,
                 GL_STREAM_DRAW);
    glGenBuffers(1, &normalIt->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalIt->vbo);
    glBufferData(GL_ARRAY_BUFFER, normalIt->buffer.size() * sizeof(Vec3f), NULL,
                 GL_STREAM_DRAW);
  }
  return ok();
}

Result OpenGlRenderer::process(const Ptr<Blackboard>& board) {
  // Get camera
  Camera cam = board->getCamera();
  Mat4x4f VP = cam.projection * cam.view;

  auto shaderPtr = board->getProperties<Shader>("shader");
  auto vertexPtr = board->getProperties<BufferVec3f>("vertex");
  auto uvPtr = board->getProperties<BufferVec2f>("uv");
  auto posPtr = board->getProperties<Transform>("transform");
  auto normPtr = board->getProperties<BufferVec3f>("normal");
  if (!shaderPtr || !vertexPtr || !uvPtr || !posPtr) {
    return error("nothing to render");
  }

  auto shaderIt = shaderPtr->buffer().begin();
  auto shaderEnd = shaderPtr->buffer().end();
  auto vertexIt = vertexPtr->buffer().begin();
  auto uvIt = uvPtr->buffer().begin();
  auto posIt = posPtr->buffer().begin();
  auto normIt = normPtr->buffer().begin();

  for (; shaderIt != shaderEnd;
       ++shaderIt, ++vertexIt, ++uvIt, ++posIt, ++normIt) {
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
    glBindBuffer(GL_ARRAY_BUFFER, vertexIt->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexIt->buffer.size() * sizeof(Vec3f), NULL,
                 GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexIt->buffer.size() * sizeof(Vec3f),
                    vertexIt->buffer.data());
    glVertexAttribPointer(0,         // attribute
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void*)0   // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvIt->vbo);
    glBufferData(GL_ARRAY_BUFFER, uvIt->buffer.size() * sizeof(Vec2f), NULL,
                 GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, uvIt->buffer.size() * sizeof(Vec2f),
                    uvIt->buffer.data());
    glVertexAttribPointer(1,         // attribute
                          2,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void*)0   // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normIt->vbo);
    glBufferData(GL_ARRAY_BUFFER, normIt->buffer.size() * sizeof(Vec3f), NULL,
                 GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, uvIt->buffer.size() * sizeof(Vec3f),
                    uvIt->buffer.data());
    glVertexAttribPointer(2,         // attribute
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          (void*)0   // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vertexIt->buffer.size());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  return ok();
}  // namespace arty

void OpenGlRenderer::release() {}

}  // namespace arty
