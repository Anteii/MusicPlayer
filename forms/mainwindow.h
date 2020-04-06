#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "player_core/player.h"
#include "controllers/listcontroller.h"
#include "static_classes/fileassistant.h"
#include "controllers/timingcontroller.h"
#include "controllers/volumecontroller.h"
#include "controllers/playercontroller.h"
#include "controllers/graphiccontroller.h"
#include "player_core/player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

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

  FileAssistant * fileAssistance = NULL;
  Player* player = NULL;
  ListController * listController = NULL;
  TimingController * timingController = NULL;
  VolumeController * volumeController = NULL;
  PlayerController * playerController = NULL;
  GraphicController * graphicController = NULL;
};
#endif // MAINWINDOW_H
