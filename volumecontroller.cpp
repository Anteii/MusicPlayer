#include "volumecontroller.h"

VolumeController::VolumeController(QObject *parent) : QObject(parent)
{

}

void VolumeController::init(QSlider * slider)
{
  this->slider = slider;
  isInited = true;
}

void VolumeController::sliderPosChanged(int val)
{
  emit volumeSet( ((float)val) / 100);
}
