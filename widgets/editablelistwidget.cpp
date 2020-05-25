#include "editablelistwidget.h"
#include <QDebug>


EditableListWidget::EditableListWidget(QWidget* parent)
    : QListWidget(parent)
{
    this->setDragEnabled(true);
    this->setDefaultDropAction(Qt::MoveAction);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    qDebug() << 1;
}
