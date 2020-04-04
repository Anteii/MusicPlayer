#ifndef OGLTEST_H
#define OGLTEST_H

#include "Visualization.h"
#include <QOpenGLFunctions_4_5_Core>


class OGLTest : public Visualization, QOpenGLFunctions_4_5_Core
{
public:
  OGLTest(OGLF * f);
  ~OGLTest();
  void draw();

  void init() override;
  void deInit() override;
  void update() override;

private:

  float * points;
  GLuint lst;
  GLuint vbo;
  GLuint vao;
  const char* vertex_shader;
  const char* fragment_shader;
  GLuint vs;
  GLuint fs;
  GLuint shader_programme;
};

#endif // OGLTEST_H
