#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>

class MySlider : public QSlider
{
  Q_OBJECT
public:
  explicit MySlider(QWidget *parent = nullptr);
protected:
  void mousePressEvent ( QMouseEvent * event );
signals:

};

#endif // MYSLIDER_H
