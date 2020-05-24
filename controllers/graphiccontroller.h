#ifndef GRAPHICCONTROLLER_H
#define GRAPHICCONTROLLER_H

#include <QOpenGLWidget>
#include <QString>
#include <thread>
#include "global_types/musicfile.h"
#include "graphics/graphic.h"
#include "graphics/Visualization.h"
#include "graphics/ogltest.h"
#include "graphics/FourierDraw.h"
#include "logger/logger.h"
#define logger_name logger
class GraphicController : public QObject
{
  Q_OBJECT
public:
  enum VisualizationTypes{
    NONE, TEST, FIRST, FOURIER
  };

  explicit GraphicController(QObject *parent = nullptr);
  void init(Graphic *);
  bool isInited();
  void delaySet(VisualizationTypes type);
  void setLogger(Logger * _logger);
  void handleChangedTrack();
  ~GraphicController();
public slots:
  void startUpdating();
  void stopUpdating();
  void setVisualization(int);
  void setPlayerController(PlayerController * pc);
signals:
  void changeVisualization(int);
  void readyToChange();
private:
  struct {
    bool _isUpdating = false;
    bool _isInited = false;
    bool _synchronizedFlag = false;
    bool _shutDown = false;
    bool _updaterIsRunning = false;
  } flags;
  void _setVisualization(int type);
  int nextVisType = 0;
  void initUpdaterThread();
  Logger * logger = NULL;

  VisualizationTypes current;
  Visualization * vis;
  Graphic * graphic;
  std::thread * updater;
  PlayerController * playerController = NULL;
};


#endif // GRAPHICCONTROLLER_H
