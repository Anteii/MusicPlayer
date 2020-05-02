#include "ogltest.h"
#include <QDebug>


OGLTest::OGLTest(OGLF * _f) : f(_f)
{
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

void OGLTest::init()
{

  f->glGenBuffers(1, &vbo);
  f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  f->glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  f->glGenVertexArrays(1, &vao);
  f->glBindVertexArray(vao);

  f->glEnableVertexAttribArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  vs = f->glCreateShader(GL_VERTEX_SHADER);
  f->glShaderSource(vs, 1, &vertex_shader, NULL);
  f->glCompileShader(vs);

  fs = f->glCreateShader(GL_FRAGMENT_SHADER);
  f->glShaderSource(fs, 1, &fragment_shader, NULL);
  f->glCompileShader(fs);

  shader_programme = f->glCreateProgram();
  f->glAttachShader(shader_programme, fs);
  f->glAttachShader(shader_programme, vs);
}

void OGLTest::deInit()
{

}

void OGLTest::update()
{
  draw();
}

void OGLTest::setPlayerController(PlayerController *ctr)
{
  pc = ctr;
}

void OGLTest::draw()
{

  //glLinkProgram(shader_programme);
  //glUseProgram(shader_programme);

  f->glEnableVertexAttribArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  f->glBindVertexArray(vao);
  f->glDrawArrays(GL_TRIANGLES, 0, 3);

  f->glDisableVertexAttribArray(0);
  f->glBindBuffer(GL_ARRAY_BUFFER, 0);
}
