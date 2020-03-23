#ifndef TIMINGCONTROLLER_H
#define TIMINGCONTROLLER_H

#include <QObject>
#include <QSlider>


class TimingController : public QObject
{
  Q_OBJECT
public:
  explicit TimingController(QObject *parent = nullptr);
  void initSlider(QSlider * list);

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
};

#endif // TIMINGCONTROLLER_H
