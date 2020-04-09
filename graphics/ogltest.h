#ifndef OGLTEST_H
#define OGLTEST_H

#include "visualization.h"

class OGLTest : public Visualization
{
public:
  OGLTest(OGLF * f);
  ~OGLTest();
  void draw();

  void init() override;
  void deInit() override;
  void update() override;
  void setMusicFile(MusicFile* musicFile) override;

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

  MusicFile * mf;
};

#endif // OGLTEST_H
