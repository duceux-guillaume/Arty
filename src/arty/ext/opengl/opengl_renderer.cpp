#include <arty/core/math.hpp>
#include <arty/core/mesh.hpp>
#include <arty/ext/opengl/gl_loader.hpp>
#include <arty/ext/opengl/opengl_renderer.hpp>
#include <arty/impl/camera_system.hpp>

namespace arty {

OpenGlRenderer::OpenGlRenderer() : _renderer() {}

OpenGlRenderer::~OpenGlRenderer() { std::cout << "ByeBye" << std::endl; }

Result OpenGlRenderer::init(Ptr<Memory> const& /*board*/) {
  return_if_error(_renderer.init());
  return ok();
}

Result OpenGlRenderer::process(const Ptr<Memory>&) {
  /*
  // Import meshes
  auto ptr = board->getProperties<Mesh>("mesh2import");
  if (ptr) {
    for (auto const& prop : *ptr) {
      MeshVbos vbo;
      _renderer.importStaticMeshNoTexture(prop.value, vbo);
      board->set(prop.entity, "mesh", prop.value);
      board->set(prop.entity, "vbo", vbo);
    }
    board->clearProperties("mesh2import");
  }

  // Get camera
  auto cam = board->getProperties<Camera>("camera")->at(0).value;

  auto vbosPtr = board->getProperties<MeshVbos>("vbo");
  auto meshPtr = board->getProperties<Mesh>("mesh");
  auto posPtr = board->getProperties<Transform>("transform");
  if (!vbosPtr || !meshPtr || !posPtr) {
    return error("nothing to render");
  }

  auto vboIt = vbosPtr->begin();
  auto vboEnd = vbosPtr->end();
  auto meshIt = meshPtr->begin();
  auto posIt = posPtr->begin();

  for (; vboIt != vboEnd; ++vboIt, ++meshIt, ++posIt) {
    if (vboIt->entity != meshIt->entity) {
      return error("vbo doesn't match entity");
    }
    if (posIt->entity != meshIt->entity) {
      return error("transform doesn't match entity");
    }
    _renderer.drawStaticMeshNoTexture(meshIt->value, posIt->value.toMat(),
                                      cam.view, cam.projection, vboIt->value);
  }
  */
  return ok();
}

void OpenGlRenderer::release() {}

static OpenGlPtrs notexture_shader;
static std::unordered_map<std::string, GLuint> notexture_light_shader;

static MeshVbos static_vbos;

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
  glGenBuffers(1, &static_vbos.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, static_vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
}

Result Renderer::drawDynMeshNoTextureNoIndices(const Mesh& m,
                                               Mat4x4f const& mvp) {
  glUseProgram(notexture_shader.program);
  glUniformMatrix4fv(notexture_shader.mvp, 1, GL_FALSE, mvp.transpose().ptr());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, static_vbos.vertex);
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
  glGenBuffers(1, &static_vbos.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, static_vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f),
               m.vertices.data(), GL_STATIC_DRAW);
}

Result Renderer::drawStaticMeshNoTextureNoIndices(const Mesh& m,
                                                  const Mat4x4f& mvp) {
  glUseProgram(notexture_shader.program);
  glUniformMatrix4fv(notexture_shader.mvp, 1, GL_FALSE, mvp.transpose().ptr());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, static_vbos.vertex);
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

void Renderer::importStaticMeshNoTexture(const Mesh& m, MeshVbos& out) {
  glGenBuffers(1, &out.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, out.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f),
               m.vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &out.normal);
  glBindBuffer(GL_ARRAY_BUFFER, out.normal);
  glBufferData(GL_ARRAY_BUFFER, m.normals.size() * sizeof(Vec3f),
               m.normals.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &out.element);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.element);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               m.indices.size() * sizeof(unsigned short), m.indices.data(),
               GL_STATIC_DRAW);
}

void Renderer::importStreamMeshNoTexture(const Mesh& m, MeshVbos& out) {
  glGenBuffers(1, &out.vertex);
  glBindBuffer(GL_ARRAY_BUFFER, out.vertex);
  glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);

  glGenBuffers(1, &out.normal);
  glBindBuffer(GL_ARRAY_BUFFER, out.normal);
  glBufferData(GL_ARRAY_BUFFER, m.normals.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
}

Result Renderer::drawStaticMeshNoTexture(const Mesh& mesh, const Mat4x4f& model,
                                         const Mat4x4f& view,
                                         const Mat4x4f& proj,
                                         const MeshVbos& vbos) {
  glUseProgram(notexture_light_shader["program"]);

  Mat4x4f mvp = proj * view * model;

  glUniformMatrix4fv(notexture_light_shader["mvp"], 1, GL_FALSE,
                     mvp.transpose().ptr());
  glUniformMatrix4fv(notexture_light_shader["model"], 1, GL_FALSE,
                     model.transpose().ptr());
  glUniformMatrix4fv(notexture_light_shader["view"], 1, GL_FALSE,
                     view.transpose().ptr());

  Vec3f lightPos({2, 2, 10});
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

Result Renderer::drawStreamMeshNoTexture(const Mesh& mesh, const Mat4x4f& model,
                                         const Mat4x4f& view,
                                         const Mat4x4f& proj,
                                         const MeshVbos& vbos) {
  glUseProgram(notexture_light_shader["program"]);

  Mat4x4f mvp = proj * view * model;

  glUniformMatrix4fv(notexture_light_shader["mvp"], 1, GL_FALSE,
                     mvp.transpose().ptr());
  glUniformMatrix4fv(notexture_light_shader["model"], 1, GL_FALSE,
                     model.transpose().ptr());
  glUniformMatrix4fv(notexture_light_shader["view"], 1, GL_FALSE,
                     view.transpose().ptr());

  Vec3f lightPos({2, 2, -10});
  glUniform3f(notexture_light_shader["light"], lightPos.x(), lightPos.y(),
              lightPos.z());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.vertex);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(Vec3f),
                  mesh.vertices.data());
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vbos.normal);
  glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(Vec3f), NULL,
               GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.normals.size() * sizeof(Vec3f),
                  mesh.vertices.data());
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
