#ifndef VOLUMECONTROLLER_H
#define VOLUMECONTROLLER_H

#include <QObject>
#include <QSlider>
#include "logger/logger.h"

class VolumeController : public QObject
{
  Q_OBJECT
public:
  explicit VolumeController(QObject *parent = nullptr);
  void init(QSlider *);
  void setVolume(int);
  void setLogger(Logger * _logger);
private:
  bool isInited = false;
  QSlider * slider = NULL;
  Logger * logger = NULL;
signals:
  void volumeChanged(float);
};

#endif // VOLUMECONTROLLER_H
