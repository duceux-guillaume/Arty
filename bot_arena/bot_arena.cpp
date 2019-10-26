#include <arty/ext/gl_loader.h>
#include <arty/ext/opengl_renderer.h>
#include <arty/ext/opengl_window.h>
#include <arty/impl/engine.h>

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
    return *this;
  }

  Factory& obj(std::string const& file) {
    _vertex.file = file;
    return *this;
  }

  Result build(Ptr<Blackboard> const& board) {
    std::string entity = board->createEntity(_name);
    board->set(entity, "transform", _tf);
    board->set(entity, "shader", _shader);
    board->set(entity, "vertex", _vertex);
    board->set(entity, "uv", _uv);
    board->set(entity, "normal", _normal);
    return ok();
  }

 private:
  std::string _name;
  Transform _tf;
  Shader _shader;
  BufferVec3f _vertex;
  BufferVec2f _uv;
  BufferVec3f _normal;
};

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Engine engine;
  engine.set_window(window)
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new OpenGlRenderer));

  Factory factory;
  auto res = factory.name("mybot")
                 .position(0.f, 0.f, 0.f)
                 .shaders("shaders/StandardShading.vertexshader.vertexshader",
                          "shaders/StandardShading.fragmentshader")
                 .texture("textures/uvmap.DDS")
                 .obj("models/bot_1.obj")
                 .build(engine.board());
  check_result(res);

  res = factory.name("mybot")
            .position(4.f, 0.f, 0.f)
            .shaders("shaders/StandardShading.vertexshader.vertexshader",
                     "shaders/StandardShading.fragmentshader")
            .texture("textures/uvmap.DDS")
            .obj("models/cube.obj")
            .build(engine.board());
  check_result(res);

  res = factory.name("suzanne")
            .position(-4.f, 0.f, 0.f)
            .shaders("shaders/StandardShading.vertexshader.vertexshader",
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
