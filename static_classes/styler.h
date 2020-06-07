#ifndef STYLER_H
#define STYLER_H

#include <QPushButton>
#include <QSlider>
#include <QString>

#include "fileassistant.h"

/*!
 * \brief The Styler class
 * This class works with external css files
 */
class Styler
{
public:
  static inline void setBtnPlay(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "playBtn"));
  }
  static inline void setBtnPause(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "pauseBtn"));
  }
  static inline void setBtnNextTrack(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "nextTrackBtn"));
  }
  static inline void setBtnPrevTrack(QPushButton* btn){
    btn->setStyleSheet(loadStyleFromFile("buttons", "prevTrackBtn"));
  }
  static inline void setBtnRandTrack(QPushButton* btn, bool isPressed = false){
    if (isPressed){
        btn->setStyleSheet(loadStyleFromFile("buttons", "randTrackBtnPressed"));
      }
    else{
        btn->setStyleSheet(loadStyleFromFile("buttons", "randTrackBtn"));
      }

  }
  static inline void setBtnRepeatTrack(QPushButton* btn, bool isPressed = false){
    if (isPressed){
        btn->setStyleSheet(loadStyleFromFile("buttons", "repeatTrackBtnPressed"));
      }
    else{
        btn->setStyleSheet(loadStyleFromFile("buttons", "repeatTrackBtn"));
      }
  }
  static inline QString loadStyleFromFile(QString const & type, QString const & fileName){
    return FileAssistant::getStyle(type, fileName);
  }
  static inline void setSliderTime(QSlider * slider){
    slider->setStyleSheet(loadStyleFromFile("sliders", "trackTimingSlider"));
  }
  static inline void setSliderVolume(QSlider * slider){
    slider->setStyleSheet(loadStyleFromFile("sliders", "volumeSlider"));
  }
};

#endif // STYLER_H
