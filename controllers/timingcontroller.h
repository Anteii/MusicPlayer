#ifndef TIMINGCONTROLLER_H
#define TIMINGCONTROLLER_H

#include <QObject>
#include <QSlider>
#include "logger/logger.h"

class TimingController : public QObject
{
  Q_OBJECT
public:
  explicit TimingController(QObject *parent = nullptr);
  void initSlider(QSlider * list);
  void setLogger(Logger * _logger);
signals:
  void sliderPosChanged(int);
public slots:
  void setValue(int);
  void capture();
  void free();
private:
  bool isCaptured = false;
  bool isInited = false;
  QSlider* slider;
  Logger * logger = NULL;
};

#endif // TIMINGCONTROLLER_H
