#include "ogltest.h"

OGLTest::OGLTest()
{
  initializeOpenGLFunctions();

  points = new float[9]{
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
   -0.5f, -0.5f,  0.0f
  };

  vbo = 0;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

  vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
  "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
  "}";

  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);
  glUseProgram(shader_programme);


  lst = glGenLists(1);
  glNewList(lst, GL_COMPILE);
    glBegin(GL_TRIANGLES);
      glColor3f(0.32, 0.857, 0.567);
      glVertex3f(-1, 1, 0);
      glVertex3f(-1, -1, 0);
      glVertex3f(0, 0, 0);
    glEnd();
  glEndList();

}

OGLTest::~OGLTest()
{
  glDeleteLists(lst, 1);
}

void OGLTest::setOGLFunctionPointer(OGLF * functions)
{
  func = functions;
}

void OGLTest::init()
{

}

void OGLTest::update()
{
  draw();
}

void OGLTest::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCallList(lst);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
