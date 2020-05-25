#include "mylist.h"

MyList::MyList(QWidget *parent) : QListWidget(parent)
{

}

void MyList::mousePressEvent(QMouseEvent *event)
{
  clearSelection();
  event->accept();
  QListWidget::mousePressEvent(event);
}
