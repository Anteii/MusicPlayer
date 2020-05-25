#ifndef EDITABLELISTWIDGET_H
#define EDITABLELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QDrag>

class EditableListWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit EditableListWidget(QWidget *parent = nullptr);
protected:
signals:

};

#endif // EDITABLELISTWIDGET_H
