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

class Transform {
 public:
  Transform() : position(), rotation(1.f), scale({1.f, 1.f, 1.f}) {}

  Vec3f position;
  Mat4x4f rotation;
  Vec3f scale;
};

struct Shader {
 public:
  unsigned int program;
};

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
    auto id = LoadShaders("shaders/TransformVertexShader.vertexshader",
                          "shaders/TextureFragmentShader.fragmentshader");
    _shader.program = id;
    return *this;
  }

  Result build(Engine& engine) {
    if (_shader.program == 0) {
      return error("error loading shaders");
    }

    Entity entity(_name);

    engine.set(entity, "transform", _tf);
    engine.set(entity, "shader", _shader);
    engine.set(entity, "vertex", _vertex);
    engine.set(entity, "uv", _uv);
    engine.set(entity, "normal", _normal);
    return ok();
  }

 private:
  std::string _name;
  Transform _tf;
  Shader _shader;
  Mesh _vertex;
  Mesh _uv;
  Mesh _normal;
};

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Engine engine;
  engine.set_window(window)
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new OpenGlRenderer));

  Factory factory;
  auto res = factory.name("mybot").position(0.f, 0.f, 0.f).build(engine);
  if (!res) {
    std::cerr << res.message() << std::endl;
    engine.stop();
    return res;
  }

  res = engine.start();
  if (!res) {
    std::cerr << res.message() << std::endl;
    engine.stop();
    return res;
  }
  res = engine.run();
  if (!res) {
    std::cerr << res.message() << std::endl;
  }
  return res;
}
