#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "controllers/graphiccontroller.h"

#define logger_name logger
namespace Ui {
  class SettingsWindow;
}

class SettingsWindow : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget *parent = nullptr);
  void setLogger(Logger * _logger);
  ~SettingsWindow();

private slots:

  void on_viz_0_clicked();
  void on_viz_2_clicked();

  void on_radioButton_clicked();

signals:
  void visTypeChanged(int);
private:
  Ui::SettingsWindow *ui;
  Logger * logger = NULL;
  GraphicController::VisualizationTypes visualizationType;
};

#endif // SETTINGSWINDOW_H
