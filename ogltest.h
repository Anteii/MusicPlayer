#ifndef OGLTEST_H
#define OGLTEST_H

#include "Visualization.h"
#include <QOpenGLFunctions_4_5_Core>


class OGLTest : public Visualization, protected QOpenGLFunctions_4_5_Core
{
public:
  OGLTest();
  ~OGLTest();
  void draw();

  void setOGLFunctionPointer(OGLF *) override;
  void init() override;
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
  OGLF * func;

};

#endif // OGLTEST_H
