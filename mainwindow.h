#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "player.h"
#include "listcontroller.h"
#include "fileassistant.h"
#include "timingcontroller.h"
#include "volumecontroller.h"


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
  void initFA();
  void initStyles();
  void initVolumeController();
  void makeConnections();

  FileAssistant * fileAssistance = NULL;
  Player* player = NULL;
  ListController * listController = NULL;
  TimingController * timingController = NULL;
  VolumeController * volumeController = NULL;
};
#endif // MAINWINDOW_H
