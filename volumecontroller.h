#ifndef VOLUMECONTROLLER_H
#define VOLUMECONTROLLER_H

#include <QObject>
#include <QSlider>


class VolumeController : public QObject
{
  Q_OBJECT
public:
  explicit VolumeController(QObject *parent = nullptr);
  void init(QSlider *);
  void sliderPosChanged(int);
private:
  bool isInited = false;
  QSlider * slider = NULL;
signals:
  void volumeSet(float);
};

#endif // VOLUMECONTROLLER_H
