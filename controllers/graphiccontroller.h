#ifndef GRAPHICCONTROLLER_H
#define GRAPHICCONTROLLER_H

#include <QOpenGLWidget>
#include <QString>
#include <thread>
#include "global_types/musicfile.h"
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
  void setMusicFile(MusicFile*);
  void setPlayer(Player * pl);
signals:
  void changeVisualization(int);
private:
  void initUpdaterThread();
  bool _isInited;
  VisualizationTypes current;
  Visualization * vis;
  Graphic * graphic;
  std::thread * updater;
  MusicFile * musicFile;
};

#endif // GRAPHICCONTROLLER_H
