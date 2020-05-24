#include "mainwindow.h"
#include "ui_mainwindow.h"
#define logger_name logger
MainWindow::MainWindow(Logger * _logger, QWidget *parent)
  : QMainWindow(parent), logger(_logger)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  init();
}

MainWindow::~MainWindow()
{
  LOG(Logger::Message, "Starting realease of MainWindow holding resources");
  delete graphicController;
  delete listController;
  delete timingController;
  delete volumeController;
  delete playerController;
  delete player;
  delete ui;
  delete fileAssistance;
  delete settingsWindow;
  LOG(Logger::Message, "All MainWindow resources were realeased");
}

void MainWindow::init()
{
  LOG(Logger::Message, "Begin MainWindow component initializations");
  FileAssistant::initMusicDir();
  FileAssistant::initPlaylistsDir();
  initSettingsWindow();
  initPlayer();
  initPlayerController();
  initGraphicController();
  initList();
  initTimingController();
  initVolumeController();
  initFA();
  initStyles();
  setEnabledToControl(false);
  LOG(Logger::Message, "End of component initializations");
  makeConnections();
  LOG(Logger::Message, "End of connections setups");
  this->setWindowIcon(QIcon(":/Windows/resources/icons/windows/mainWindow.png"));
  this->setWindowTitle("MEGAPLAYER");
}

void MainWindow::initList()
{
  PlayList::createBasePlaylist();
  listController = new ListController(this);
  listController->initList(ui->listWidget);
  listController->loadPlaylists();
  // init horizontal size of splitter

  int w = ui->splitter->width();
  QList<int> s = {w, w * 6};
  ui->splitter->setSizes(s);

  ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

}

void MainWindow::initTimingController()
{
  timingController = new TimingController;
  timingController->setLogger(logger);
  timingController->initSlider(ui->slider);
}

void MainWindow::initPlayer()
{
  player = new Player();
  player->setLogger(logger);
}

void MainWindow::initPlayerController()
{
  playerController = new PlayerController();
  playerController->setLogger(logger);
  playerController->initPlayer(player);
}

void MainWindow::initFA()
{
  // May be FileAssistanse::init();
}

void MainWindow::initStyles()
{
  Styler::setBtnPlay(ui->playPauseBtn);
  Styler::setBtnNextTrack(ui->nextTrackBtn);
  Styler::setBtnPrevTrack(ui->prevTrackBtn);
  Styler::setBtnRandTrack(ui->randTrackBtn);
  Styler::setBtnRepeatTrack(ui->replayTrackBtn);
  Styler::setSliderTime(ui->slider);
  Styler::setSliderVolume(ui->volumeSlider);
}

void MainWindow::initVolumeController()
{
  volumeController = new VolumeController(this);
  volumeController->setLogger(logger);
  volumeController->init(ui->volumeSlider);
}

void MainWindow::initGraphicController()
{
  graphicController = new GraphicController(this);
  graphicController->setLogger(logger);
  graphicController->init(ui->openGLWidget);
  graphicController->setPlayerController(playerController);
}

void MainWindow::makeConnections()
{
  // Трек сам закончился
  connect(playerController,
          &PlayerController::trackEnded,
          [&](){
      playerController->pause();
      graphicController->stopUpdating();
      if (playerController->playNextTrack()){
          setEnabledToControl(false);
          Styler::setBtnPlay(ui->playPauseBtn);
          return;
        }
      else{
          setEnabledToControl(true);
          Styler::setBtnPause(ui->playPauseBtn);
        }
      graphicController->startUpdating();
      if (playerController->getPlaylist() != NULL &&
        *(playerController->getPlaylist()) == *(listController->getPlayList()))
      {
        listController->setSelected(playerController->getCurrentTrack());
      }
    });
  // Двойной клик в плейлисте обрабатывается здесь on_listWidget_itemDoubleClicked
  // -------------- SAFE -------------------
  // Play/pause btn
  connect(
        ui->playPauseBtn,
        &QPushButton::clicked,
        [&](){
          playerController->playPause();
          if (player->isPlaying()){
              Styler::setBtnPause(ui->playPauseBtn);
            }
          else{
              Styler::setBtnPlay(ui->playPauseBtn);
            }
        }
        );
  // nextTrackBtn
  connect(
        ui->nextTrackBtn,
        &QPushButton::clicked,
        [&](){
      playerController->pause();
      graphicController->stopUpdating();
      if (playerController->playNextTrack()){
          setEnabledToControl(false);
          Styler::setBtnPlay(ui->playPauseBtn);
          return;
        }
      else{
          setEnabledToControl(true);
          Styler::setBtnPause(ui->playPauseBtn);
        }
      graphicController->startUpdating();
      if (playerController->getPlaylist() != NULL &&
        playerController->getPlaylist()->getName() == listController->getPlayList()->getName())
      {
          listController->setPlaylist(playerController->getPlaylist()->clone());
          listController->clear();
          listController->addGoBack();
          listController->loadTracks();
          listController->setSelected(playerController->getCurrentTrack());
      }
    });
  // prevTrackBtn
  connect(
        ui->prevTrackBtn,
        &QPushButton::clicked,
        [&](){
      playerController->pause();
      graphicController->stopUpdating();
      if (playerController->playPrevTrack()){
          setEnabledToControl(false);
          Styler::setBtnPlay(ui->playPauseBtn);
          return;
        }
      else{
          setEnabledToControl(true);
          Styler::setBtnPause(ui->playPauseBtn);
        }
      graphicController->startUpdating();
      if (playerController->getPlaylist() != NULL &&
        playerController->getPlaylist()->getName() == listController->getPlayList()->getName())
      {
          qDebug() << "ASDHFGASHFGALSDHFGLHID";
        listController->setPlaylist(playerController->getPlaylist()->clone());
        listController->clear();
        listController->addGoBack();
        listController->loadTracks();
        listController->setSelected(playerController->getCurrentTrack());
      }
    });
  // durationChanged player
  connect(
        this->playerController,
        &PlayerController::trackDurationChanged,
        ui->slider,
        &QSlider::setMaximum
        );

  // replayTrackBtn
  connect(
        ui->replayTrackBtn,
        &QPushButton::clicked,
        [&](){
            bool flag = !(playerController->isLoopedTrack());
            Styler::setBtnRepeatTrack(ui->replayTrackBtn, flag);
            playerController->setLoopedTrack(flag);
        });
  // randTrackBtn
  connect(
        ui->randTrackBtn,
        &QPushButton::clicked,
        [&](){
      bool flag = !(playerController->isRandTrack());
      Styler::setBtnRandTrack(ui->randTrackBtn, flag);
      playerController->setRandTrack(flag);
    });
  // sliderPressed slider
  connect(
        ui->slider,
        &QSlider::sliderPressed,
        timingController,
        &TimingController::capture
        );
  // sliderRelease slider
  connect(
        ui->slider,
        &QSlider::sliderReleased,
        timingController,
        &TimingController::free
        );
  // sliderRelease slider -> setTime player
  connect(
        timingController,
        &TimingController::sliderPosChanged,
        playerController,
        &PlayerController::setTime
        );
  // правая кнопка на песне/плейлисте
  connect(
        ui->listWidget,
        SIGNAL(customContextMenuRequested(const QPoint &)),
        listController,
        SLOT(ProvideContextMenu(const QPoint &))
        );
  // Volume slider change
  connect(
        ui->volumeSlider,
        &QSlider::valueChanged,
        volumeController,
        &VolumeController::setVolume
        );
  //
  connect(
        volumeController,
        &VolumeController::volumeChanged,
        playerController,
        &PlayerController::setVolume
        );
  connect(settingsWindow,
          &SettingsWindow::visTypeChanged,
          graphicController,
          &GraphicController::setVisualization
    );
  // positionChanged player -> update label-timer
  connect(
        this->playerController,
        &PlayerController::trackPositionChanged,
        [&](int time){
      timingController->setValue(time);
      int m, s;
      s = (time / 1000) % 60;
      m = (time / 60000) % 60;
      QString ttime;
      ttime.sprintf("%.2i:%.2i", m, s);
      ui->currentTime->setText(ttime);
    });
}

void MainWindow::initSettingsWindow()
{
  settingsWindow = new SettingsWindow(this);
}

void MainWindow::initLogger()
{
  logger = new Logger();
}

void MainWindow::setEnabledToControl(bool flag)
{
  ui->playPauseBtn->setEnabled(flag);
  ui->nextTrackBtn->setEnabled(flag);
  ui->prevTrackBtn->setEnabled(flag);
  ui->slider->setEnabled(flag);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString type = item->data(Qt::UserRole).toString();
    QString text = item->text();
    LOG(Logger::Message, "Double click on list");
    if (type == "playlist"){

        PlayList* pl = new PlayList(fileAssistance->getPlaylistPath(text).toStdString());

        listController->clear();
        listController->setPlaylist(pl);
        listController->addGoBack();
        listController->loadTracks();

        if (player->getPlaylist() != NULL && *pl == *(player->getPlaylist())){
            listController->setSelected(playerController->getPlaylist()->getPosition());
          }

        ui->ListType->setText("Tracks");
      }
    else if (type.startsWith("command")){
        if (type.split("_")[1] == "goBack"){
            if (listController->whatDisplays() == ListController::TRACKS){
              listController->clear();
              listController->loadPlaylists();
              ui->ListType->setText("Playlists");
            }
          }
      }
    else if (type == "track"){

        // Configure playlist
        PlayList * temp = listController->getPlayList();
        auto index = ui->listWidget->selectionModel()->selectedIndexes().first().row() - 1;
        temp->setPosition(index);
        // Change playlist if need
        if (playerController->getPlaylist() == NULL || *(playerController->getPlaylist()) != *(temp)){
            playerController->setPlaylist(temp->clone());
          }
        else{
            playerController->getPlaylist()->setPosition(index);
          }
        if (playerController->start()){
            setEnabledToControl(false);
            Styler::setBtnPlay(ui->playPauseBtn);
          }
        else{
            setEnabledToControl(true);
            Styler::setBtnPause(ui->playPauseBtn);
          }

      }
}

void MainWindow::on_actionVisualization_settings_triggered()
{
  LOG(Logger::Message, "Settings window was triggered from menu tab");
  settingsWindow->show();
}
