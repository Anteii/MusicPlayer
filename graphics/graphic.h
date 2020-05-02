#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QDebug>
#include <thread>

#include <player_core/player.h>

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
  void setPlayerController(PlayerController * pctr);
  void deInitEffect();
  bool isInited();
  bool isInitedEffect();
  void setRedFlag(bool);
  bool getRedFlag();
  bool isUpdating();
  void clear();
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
  bool _isUpdating = false;
  QPainter * _painter;
  bool _isInited;
  bool _isInitedEffect;
  bool redFlag = false;
  PlayerController * playerController;
};

#endif // GRAPHIC_H
