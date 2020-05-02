#include "volumecontroller.h"

VolumeController::VolumeController(QObject *parent) : QObject(parent)
{

}

void VolumeController::init(QSlider * slider)
{
  this->slider = slider;
  isInited = true;
}

void VolumeController::setVolume(int val)
{
  emit volumeChanged( ((float)val) / 100);
}

void VolumeController::setLogger(Logger *_logger)
{
  logger = _logger;
}
