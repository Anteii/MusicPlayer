#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "forms/settingswindow.h"
#include "player_core/player.h"
#include "controllers/listcontroller.h"
#include "static_classes/fileassistant.h"
#include "controllers/timingcontroller.h"
#include "controllers/volumecontroller.h"
#include "controllers/playercontroller.h"
#include "controllers/graphiccontroller.h"
#include "player_core/player.h"
#include "logger/logger.h"
#include "decoder/musicfiledecoder.h"
#include "graphics/graphic.h"
#include "static_classes/styler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Logger * _logger, QWidget *parent = nullptr);
  ~MainWindow();
signals:
  void askSetEnabledToControls(bool);
  void askSetPauseBtn();
  void askSetPlayBtn();
private slots:
  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
  void on_actionVisualization_settings_triggered();
  void setEnabledToControls(bool);
  void setPauseBtn();
  void setPlayBtn();
private:
  Ui::MainWindow *ui;
  void init();
  void initList();
  void initTimingController();
  void initPlayer();
  void initPlayerController();
  void initFA();
  void initStyles();
  void initVolumeController();
  void initGraphicController();
  void makeConnections();
  void initSettingsWindow();
  void initLogger();
  void setEnabledToControl(bool);
  FileAssistant * fileAssistance = NULL;
  Player* player = NULL;
  ListController * listController = NULL;
  TimingController * timingController = NULL;
  VolumeController * volumeController = NULL;
  PlayerController * playerController = NULL;
  GraphicController * graphicController = NULL;
  SettingsWindow * settingsWindow = NULL;
  Logger * logger = NULL;
};
#endif // MAINWINDOW_H
