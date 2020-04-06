#include "timingcontroller.h"
#include <QDebug>
TimingController::TimingController(QObject *parent) : QObject(parent)
{

}

void TimingController::initSlider(QSlider *slider)
{
  this->slider = slider;
  isInited = true;
}

void TimingController::setValue(int val)
{
  if (isCaptured) return;
  slider->setValue(val);
}

void TimingController::capture()
{
  isCaptured = true;
}

void TimingController::free()
{
  isCaptured = false;
  emit sliderPosChanged(slider->value());
}
