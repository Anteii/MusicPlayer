#include "ogltest.h"
#include <QDebug>


OGLTest::OGLTest(OGLF * f) : QOpenGLFunctions_4_3_Core(*f)
{
  //initializeOpenGLFunctions();
  points = new float[9]{
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
   -0.5f, -0.5f,  0.0f
  };

  vbo = 0;

  vao = 0;

  vertex_shader =
  "#version 400\n"
  "in vec3 vp;"
  "void main() {"
  "  gl_Position = vec4(vp, 1.0);"
  "}";

  fragment_shader =
  "#version 400\n"
  "out vec4 frag_colour;"
  "void main() {"
  "  frag_colour = vec4(0.5, 0.6, 0.5, 1.0);"
  "}";

}

OGLTest::~OGLTest()
{
  glDeleteLists(lst, 1);
}

void OGLTest::init()
{
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
}

void OGLTest::deInit()
{

}

void OGLTest::update()
{
  draw();
}

void OGLTest::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLinkProgram(shader_programme);
  glUseProgram(shader_programme);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
