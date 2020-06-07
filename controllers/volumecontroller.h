#ifndef VOLUMECONTROLLER_H
#define VOLUMECONTROLLER_H

#include <QObject>
#include <QSlider>
#include "logger/logger.h"

/*!
 * \brief The cvolume controller class
 * This controller operate with QSlider
 *  wich set volume to player through signal-slot
 */
class VolumeController : public QObject
{
  Q_OBJECT

public:
  //! Constructor
  explicit VolumeController(QObject *parent = nullptr);
  //! Set volume slider
  void init(QSlider *);
  //! Emit event to change volume
  void setVolume(int);
  //! Set logger
  void setLogger(Logger * _logger);

signals:
  void volumeChanged(float);

private:
  bool isInited = false;
  QSlider * slider = NULL;
  Logger * logger = NULL;
};

#endif // VOLUMECONTROLLER_H
