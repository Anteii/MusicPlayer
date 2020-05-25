#ifndef MYLIST_H
#define MYLIST_H

#include <QWidget>
#include <QListWidget>
#include <QMouseEvent>
#include <QDebug>
class MyList : public QListWidget
{
  Q_OBJECT
public:
  explicit MyList(QWidget *parent = nullptr);
protected:
  void mousePressEvent ( QMouseEvent * event );
signals:

};

#endif // MYLIST_H
