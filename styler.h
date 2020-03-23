#ifndef STYLER_H
#define STYLER_H

#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QString>
#include "fileassistant.h"


class Styler : public QObject
{
  Q_OBJECT
public:
  explicit Styler(QObject *parent = nullptr);
  static void setBtnPlay(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "playBtn"));
  }
  static void setBtnPause(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "pauseBtn"));
  }
  static void setBtnNextTrack(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "nextTrackBtn"));
  }
  static void setBtnPrevTrack(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "prevTrackBtn"));
  }
  static void setBtnRandTrack(QPushButton* btn, bool isPressed = false){
    if (isPressed){
        btn->setStyleSheet(loadStyleFromFile("buttons", "randTrackBtnPressed"));
        //return;
      }
    else{
        btn->setStyleSheet(loadStyleFromFile("buttons", "randTrackBtn"));
      }

  }
  static void setBtnRepeatTrack(QPushButton* btn, bool isPressed = false){
    if (isPressed){
        btn->setStyleSheet(loadStyleFromFile("buttons", "repeatTrackBtnPressed"));
      }
    else{
        btn->setStyleSheet(loadStyleFromFile("buttons", "repeatTrackBtn"));
      }
  }
  static QString loadStyleFromFile(QString type, QString fileName){
    return FileAssistant::getStyle(type, fileName);
  }
  static void setSliderTime(QSlider * slider){
    slider->setStyleSheet(loadStyleFromFile("sliders", "trackTimingSlider"));
  }
  static void setSliderVolume(QSlider * slider){
    slider->setStyleSheet(loadStyleFromFile("sliders", "volumeSlider"));
  }
signals:

};

#endif // STYLER_H
