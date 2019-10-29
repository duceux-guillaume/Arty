#include <arty/ext/gl_loader.h>
#include <arty/ext/opengl_renderer.h>
#include <arty/ext/opengl_window.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
#include <arty/impl/camera_system.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace arty;

class Factory {
 public:
  Factory& name(std::string const& name) {
    _name = name;
    return *this;
  }

  Factory& position(float x, float y, float z) {
    _tf.position.x() = x;
    _tf.position.y() = y;
    _tf.position.z() = z;
    return *this;
  }

  Factory& scale(float x, float y, float z) {
    _tf.scale.x() = x;
    _tf.scale.y() = y;
    _tf.scale.z() = z;
    return *this;
  }

  Factory& shaders(std::string const& vertexshader,
                   std::string const& fragment) {
    _shader.vertex = vertexshader;
    _shader.fragment = fragment;
    return *this;
  }

  Factory& texture(std::string const& file) {
    _shader.textureFile = file;
    _mesh.texture_file = file;
    return *this;
  }

  Factory& obj(std::string const& file) {
    _mesh.model_file = file;
    return *this;
  }

  Result build(Ptr<Blackboard> const& board) {
    std::string entity = board->createEntity(_name);
    board->set(entity, "transform", _tf);
    board->set(entity, "shader", _shader);
    board->set(entity, "mesh", _mesh);
    return ok();
  }

 private:
  std::string _name;
  Transform _tf;
  Shader _shader;
  Mesh _mesh;
};

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Engine engine;
  engine.set_window(window)
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new OpenGlRenderer));

  Factory factory;
  auto res = factory.name("bot")
                 .position(0.f, 0.f, 0.f)
                 .shaders("shaders/StandardShading.vertexshader",
                          "shaders/StandardShading.fragmentshader")
                 .obj("models/bot_1.obj")
                 .build(engine.board());
  check_result(res);

  res = factory.name("cube")
            .position(4.f, 0.f, 0.f)
            .shaders("shaders/StandardShading.vertexshader",
                     "shaders/StandardShading.fragmentshader")
            .texture("textures/uvtemplate.DDS")
            .obj("models/cube.obj")
            .build(engine.board());
  check_result(res);

  res = factory.name("suzanne")
            .position(-4.f, 0.f, 0.f)
            .shaders("shaders/StandardShading.vertexshader",
                     "shaders/StandardShading.fragmentshader")
            .texture("textures/uvmap.DDS")
            .obj("models/suzanne.obj")
            .build(engine.board());
  check_result(res);

  res = engine.start();
  if (!res) {
    std::cerr << res.message() << std::endl;
    engine.stop();
    return res;
  }

  res = engine.run();
  check_result(res);
  return res;
}
