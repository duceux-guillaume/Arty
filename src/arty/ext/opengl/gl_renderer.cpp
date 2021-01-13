#include <arty/ext/opengl/gl_renderer.hpp>

namespace arty {

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

void GlRenderer::create(Entity const& e, Rectangle const& rect) {
  // Create Vertex Array Object
  glGenVertexArrays(1, &_metadatas[e].vao);
  glBindVertexArray(_metadatas[e].vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  glGenBuffers(1, &_metadatas[e].vbo);

  GLfloat vertices[] = {
      rect.topLeft().x(),     rect.topLeft().y(),
      rect.fillColor.x(),     rect.fillColor.y(),
      rect.fillColor.z(),  // Top-left
      rect.topRight().x(),    rect.topRight().y(),
      rect.fillColor.x(),     rect.fillColor.y(),
      rect.fillColor.z(),  // Top-right
      rect.bottomRight().x(), rect.bottomRight().y(),
      rect.fillColor.x(),     rect.fillColor.y(),
      rect.fillColor.z(),  // Bottom-right
      rect.bottomLeft().x(),  rect.bottomLeft().y(),
      rect.fillColor.x(),     rect.fillColor.y(),
      rect.fillColor.z()  // Bottom-left
  };

  glBindBuffer(GL_ARRAY_BUFFER, _metadatas[e].vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create an element array
  glGenBuffers(1, &_metadatas[e].ebo);

  GLuint elements[] = {0, 1, 2, 2, 3, 0};

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _metadatas[e].ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
               GL_STATIC_DRAW);

  // Create and compile the vertex shader
  _metadatas[e].vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(_metadatas[e].vertexShader, 1, &vertexSource, NULL);
  glCompileShader(_metadatas[e].vertexShader);

  // Create and compile the fragment shader
  _metadatas[e].fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(_metadatas[e].fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(_metadatas[e].fragmentShader);

  // Link the vertex and fragment shader into a shader program
  _metadatas[e].shaderProgram = glCreateProgram();
  glAttachShader(_metadatas[e].shaderProgram, _metadatas[e].vertexShader);
  glAttachShader(_metadatas[e].shaderProgram, _metadatas[e].fragmentShader);
  glBindFragDataLocation(_metadatas[e].shaderProgram, 0, "outColor");
  glLinkProgram(_metadatas[e].shaderProgram);
  glUseProgram(_metadatas[e].shaderProgram);

  // Specify the layout of the vertex data
  _metadatas[e].posAttrib =
      glGetAttribLocation(_metadatas[e].shaderProgram, "position");
  glEnableVertexAttribArray(_metadatas[e].posAttrib);
  glVertexAttribPointer(_metadatas[e].posAttrib, 2, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), 0);

  _metadatas[e].colAttrib =
      glGetAttribLocation(_metadatas[e].shaderProgram, "color");
  glEnableVertexAttribArray(_metadatas[e].colAttrib);
  glVertexAttribPointer(_metadatas[e].colAttrib, 3, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void GlRenderer::release(Entity const& e) {
  if (_metadatas.count(e) == 0) {
    return;
  }
  glDeleteProgram(_metadatas[e].shaderProgram);
  glDeleteShader(_metadatas[e].fragmentShader);
  glDeleteShader(_metadatas[e].vertexShader);
  glDeleteBuffers(1, &_metadatas[e].ebo);
  glDeleteBuffers(1, &_metadatas[e].vbo);
  glDeleteVertexArrays(1, &_metadatas[e].vao);
}

void GlRenderer::release() {
  for (auto const& data : _metadatas) {
    release(data.first);
  }
}

void GlRenderer::draw(Entity const& e) {
  if (_metadatas.count(e) == 0) {
    return;
  }

  glBindVertexArray(_metadatas[e].vao);
  glBindBuffer(GL_ARRAY_BUFFER, _metadatas[e].vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _metadatas[e].ebo);
  glUseProgram(_metadatas[e].shaderProgram);
  glEnableVertexAttribArray(_metadatas[e].posAttrib);
  glEnableVertexAttribArray(_metadatas[e].colAttrib);

  // Draw a rectangle from the 2 triangles using 6 indices
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool GlRenderer::has(Entity const& e) { return _metadatas.count(e) > 0; }
}  // namespace arty