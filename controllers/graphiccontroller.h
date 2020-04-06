#ifndef GRAPHICCONTROLLER_H
#define GRAPHICCONTROLLER_H

#include <QOpenGLWidget>
#include <QString>
#include <thread>
#include "graphics/graphic.h"
#include "graphics/Visualization.h"
#include "graphics/ogltest.h"

class GraphicController : public QObject
{
  Q_OBJECT
public:
  enum VisualizationTypes{
    NONE, TEST
  };

  explicit GraphicController(QObject *parent = nullptr);
  void init(Graphic *);
  bool isInited();
  void delaySet(VisualizationTypes type);
public slots:
  void setVisualization(int);
signals:
  void changeVisualization(int);
private:
  bool _isInited;
  VisualizationTypes current;
  Visualization * vis;
  Graphic * graphic;
};

#endif // GRAPHICCONTROLLER_H
