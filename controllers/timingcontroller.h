#ifndef TIMINGCONTROLLER_H
#define TIMINGCONTROLLER_H

#include <QObject>
#include <QSlider>
#include "logger/logger.h"

/*!
 * \brief The time scroll bar controller class
 *
 * This controller operate with QSlider
 *  wich set time offset to player through signal-slot
 *
 */
class TimingController : public QObject
{
  Q_OBJECT

public:
  //! Constructor
  explicit TimingController(QObject *parent = nullptr);
  //! Set timing slider
  void initSlider(QSlider * list);
  //! Set logger
  void setLogger(Logger * _logger);

signals:
  void sliderPosChanged(int);

public slots:
  //! Set value to slider
  //! @param val value
  void setValue(int val);
  //! Register case of user drag slider
  void capture();
  //! Register case of user drop slider
  void free();
private:
  bool isCaptured = false;
  bool isInited = false;
  QSlider* slider;
  Logger * logger = NULL;
};

#endif // TIMINGCONTROLLER_H
