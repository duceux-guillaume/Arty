#include <arty/core/math.h>
#include <arty/ext/gl_loader.h>
#include <arty/ext/opengl_renderer.h>

#include <arty/core/mesh.hpp>

namespace arty {

OpenGlRenderer::OpenGlRenderer()
    : _program_id(0), _vertexbuffer(0), _vertexarrayid(0) {}

OpenGlRenderer::~OpenGlRenderer() { std::cout << "ByeBye" << std::endl; }

Result OpenGlRenderer::init(Ptr<Blackboard> const& /*board*/) {
  glGenVertexArrays(1, &_vertexarrayid);
  glBindVertexArray(_vertexarrayid);
  return ok();
}

Result OpenGlRenderer::process(const Ptr<Blackboard>& /*board*/) {
  // Get camera
  /*
  Camera cam = board->getCamera();
  Mat4x4f VP = cam.projection * cam.view;

  auto shaderPtr = board->getProperties<OpenGlPtrs>("shader");
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
        glUniformMatrix4fv(shaderIt->matrixId, 1, GL_FALSE,
       transpose(mvp).ptr()); glUniformMatrix4fv(shaderIt->modelId, 1,
       GL_FALSE, transpose(model).ptr()); glUniformMatrix4fv(shaderIt->viewId,
       1, GL_FALSE, transpose(cam.view).ptr());

        Vec3f lightPos{4.f, 4.f, 4.f};
        glUniform3f(shaderIt->lightId, lightPos.x(), lightPos.y(),
       lightPos.z());

        if (meshIt->hasTexture()) {
          // Bind our texture in Texture Unit 0
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, shaderIt->texture);
          // Set our "myTextureSampler" sampler to use Texture Unit 0
          glUniform1i(shaderIt->textureId, 0);
        }

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, meshIt->vertices_vbo);
        glBufferData(GL_ARRAY_BUFFER, meshIt->vertices.size() * sizeof(Vec3f),
       NULL, GL_STREAM_DRAW); glBufferSubData(GL_ARRAY_BUFFER, 0,
       meshIt->vertices.size() * sizeof(Vec3f), meshIt->vertices.data());
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
          glBufferData(GL_ARRAY_BUFFER, meshIt->uvs.size() * sizeof(Vec2f),
       NULL, GL_STREAM_DRAW); glBufferSubData(GL_ARRAY_BUFFER, 0,
       meshIt->uvs.size() * sizeof(Vec2f), meshIt->uvs.data());
          glVertexAttribPointer(1,         // attribute
                                2,         // size
                                GL_FLOAT,  // type
                                GL_FALSE,  // normalized?
                                0,         // stride
                                (void*)0   // array buffer offset
          );
        } else if (meshIt->hasColors()) {
          glEnableVertexAttribArray(1);
          glBindBuffer(GL_ARRAY_BUFFER, meshIt->colors_vbo);
          glBufferData(GL_ARRAY_BUFFER, meshIt->colors.size() * sizeof(Vec4f),
       NULL, GL_STREAM_DRAW); glBufferSubData(GL_ARRAY_BUFFER, 0,
       meshIt->colors.size() * sizeof(Vec4f), meshIt->colors.data());
          glVertexAttribPointer(1,         // attribute
                                4,         // size
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
          glBufferData(GL_ARRAY_BUFFER, meshIt->normals.size() *
       sizeof(Vec3f), NULL, GL_STREAM_DRAW); glBufferSubData(GL_ARRAY_BUFFER,
       0, meshIt->normals.size() * sizeof(Vec3f), meshIt->normals.data());
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
        if (meshIt->hasTexture() || meshIt->hasColors()) {
          glDisableVertexAttribArray(1);
        }
        if (meshIt->hasNormals()) {
          glDisableVertexAttribArray(2);
        }
  }
        */

  return ok();
}  // namespace arty

void OpenGlRenderer::release() {}

static OpenGlPtrs notexture_shader;
static std::unordered_map<std::string, GLuint> notexture_light_shader;

static MeshVbos vbos;

Result Renderer::init() {
  glGenVertexArrays(1, &notexture_shader.array);
  glBindVertexArray(notexture_shader.array);

  // SingleColor program
  {
    notexture_shader.program =
        LoadShaders("../shaders/SimpleTransform.vertexshader",
                    "../shaders/SingleColor.fragmentshader");
    if (notexture_shader.program == 0) {
      return error("error opening program file");
    }
    notexture_shader.mvp =
        glGetUniformLocation(notexture_shader.program, "MVP");
  }

  // SingleColorWithLight program
  {
    notexture_light_shader["program"] =
        LoadShaders("../shaders/SingleColorWithLight.vertexshader",
                    "../shaders/SingleColorWithLight.fragmentshader");
    if (notexture_light_shader["program"] == 0) {
      return error("error opening program file");
    }
    notexture_light_shader["mvp"] =
        glGetUniformLocation(notexture_light_shader["program"], "MVP");
    notexture_light_shader["view"] =
        glGetUniformLocation(notexture_light_shader["program"], "V");
    notexture_light_shader["model"] =
        glGetUniformLocation(notexture_light_shader["program"], "M");
    notexture_light_shader["light"] = glGetUniformLocation(
        notexture_light_shader["program"], "LightPosition_worldspace");
  }

  return ok();
}

void Renderer::release() {}

void Renderer::importDynMeshNoTextureNoIndices(const Mesh& m) {
  glGenBuffers(1, &vbos.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
}

Result Renderer::drawDynMeshNoTextureNoIndices(const Mesh& m,
                                               Mat4x4f const& mvp) {
  glUseProgram(notexture_shader.program);
  glUniformMatrix4fv(notexture_shader.mvp, 1, GL_FALSE, transpose(mvp).ptr());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, m.vertices.size() * sizeof(Vec3f),
                  m.vertices.data());
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );
  glDrawArrays(GL_TRIANGLES, 0, m.vertices.size());
  glDisableVertexAttribArray(0);

  return ok();
}

void Renderer::importStaticMeshNoTextureNoIndices(const Mesh& m) {
  glGenBuffers(1, &vbos.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f),
               m.vertices.data(), GL_STATIC_DRAW);
}

Result Renderer::drawStaticMeshNoTextureNoIndices(const Mesh& m,
                                                  const Mat4x4f& mvp) {
  glUseProgram(notexture_shader.program);
  glUniformMatrix4fv(notexture_shader.mvp, 1, GL_FALSE, transpose(mvp).ptr());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );
  glDrawArrays(GL_TRIANGLES, 0, m.vertices.size());
  glDisableVertexAttribArray(0);

  return ok();
}

void Renderer::importStaticMeshNoTexture(const Mesh& m) {
  glGenBuffers(1, &vbos.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f),
               m.vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &vbos.normal);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.normal);
  glBufferData(GL_ARRAY_BUFFER, m.normals.size() * sizeof(Vec3f),
               m.normals.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &vbos.element);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.element);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               m.indices.size() * sizeof(unsigned short), m.indices.data(),
               GL_STATIC_DRAW);
}

Result Renderer::drawStaticMeshNoTexture(const Mesh& mesh, const Mat4x4f& model,
                                         const Mat4x4f& view,
                                         const Mat4x4f& proj) {
  glUseProgram(notexture_light_shader["program"]);

  Mat4x4f mvp = proj * view * model;

  glUniformMatrix4fv(notexture_light_shader["mvp"], 1, GL_FALSE,
                     transpose(mvp).ptr());
  glUniformMatrix4fv(notexture_light_shader["model"], 1, GL_FALSE,
                     transpose(model).ptr());
  glUniformMatrix4fv(notexture_light_shader["view"], 1, GL_FALSE,
                     transpose(view).ptr());

  Vec3f lightPos({2, 2, -10});
  glUniform3f(notexture_light_shader["light"], lightPos.x(), lightPos.y(),
              lightPos.z());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.normal);
  glVertexAttribPointer(1,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  return ok();
}

}  // namespace arty
