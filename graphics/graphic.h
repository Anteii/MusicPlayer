#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QDebug>
#include <thread>

#include "ogltest.h"

// Baisc class wich will be containing different visualisations
class Graphic : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
  Q_OBJECT

public:
  explicit Graphic(QWidget *parent = nullptr);

  OGLF* getOGLF();
  QOpenGLContext * getContext();

  void setEffect(Visualization * ef);
  void initEffect();
  void updateMusicFile(MusicFile * musicFile);
  void deInitEffect();
  bool isInited();
  bool isInitedEffect();

private:
  Visualization * effect;

  void renderText(double x, double y, double z, const QString & str, const QFont & font, int listBase);
  GLint project(GLdouble objx, GLdouble objy, GLdouble objz,
                const GLdouble model[16], const GLdouble proj[16],
                const GLint viewport[4],
                GLdouble * winx, GLdouble * winy, GLdouble * winz);
  void transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]);

protected:
  void initializeGL() override;
  void resizeGL(int h, int w) override;
  void paintGL() override;

signals:

private:
  QPainter * _painter;
  bool _isInited;
  bool _isInitedEffect;
};

#endif // GRAPHIC_H
