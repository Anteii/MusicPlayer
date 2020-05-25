#include "myslider.h"

MySlider::MySlider(QWidget *parent) : QSlider(parent)
{

}

void MySlider::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
      {
          if (orientation() == Qt::Vertical)
              setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
          else
              setValue(minimum() + ((float)(maximum()-minimum()) * event->x()) / width() ) ;
          event->accept();
      }
      QSlider::mousePressEvent(event);
}
