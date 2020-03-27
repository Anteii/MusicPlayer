#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QPainter>
#include "Visualization.h"
#include "ogltest.h"

// Baisc class wich will be containing different visualisations
class Graphic : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
  Q_OBJECT

public:
  explicit Graphic(QWidget *parent = nullptr);
  void setEffect(Visualization * ef);

private:
  Visualization * effect = NULL;

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

};

#endif // GRAPHIC_H
