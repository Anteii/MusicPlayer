#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QOpenGLFunctions_4_3_Core>

#define OGLF QOpenGLFunctions_4_3_Core


class Visualization : public OGLF{
public:
  explicit Visualization(OGLF * f) : OGLF(*f){};
  virtual void init() = 0;
  virtual void deInit() = 0;
  virtual void update() = 0;
};

#endif // VISUALIZATION_H
