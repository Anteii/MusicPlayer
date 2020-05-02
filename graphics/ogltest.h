#ifndef OGLTEST_H
#define OGLTEST_H

#include "visualization.h"

class OGLTest : public Visualization
{
public:
  OGLTest(OGLF * _f);
  void draw();

  void init() override;
  void deInit() override;
  void update() override;
  void setPlayerController(PlayerController *ctr) override;
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
  Player * player;
  PlayerController * pc;
  OGLF * f;
};

#endif // OGLTEST_H
