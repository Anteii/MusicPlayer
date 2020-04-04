#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decoder/musicfiledecoder.h"
#include "graphics/graphic.h"
#include "static_classes/styler.h"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  init();
}

MainWindow::~MainWindow()
{
  delete player;
  delete listController;
  delete ui;
}

void MainWindow::init()
{
  FileAssistant::initMusicDir();
  FileAssistant::initPlaylistsDir();

  initPlayer();
  initPlayerController();
  initList();
  initTimingController();
  initVolumeController();
  initFA();
  makeConnections();

  this->setWindowIcon(QIcon(":/Windows/resources/icons/windows/mainWindow.png"));
  this->setWindowTitle("MEGAPLAYER");
  //ui->openGLWidget->setEffect(new OGLTest(ui->openGLWidget->getOGLF()));
  initStyles();
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
  timingController->initSlider(ui->slider);
}

void MainWindow::initPlayer()
{
  player = new Player();
}

void MainWindow::initPlayerController()
{
  playerController = new PlayerController();

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
  volumeController = new VolumeController;
  volumeController->init(ui->volumeSlider);
}

void MainWindow::makeConnections()
{
  // Play/pause btn -> playPause player
  connect(
        ui->playPauseBtn,
        SIGNAL (clicked(void)),
        this->playerController,
        SLOT ( playPause(void) ));
  // Play/pause btn -> switch btn style
  connect(
        ui->playPauseBtn,
        &QPushButton::clicked,
        ui->playPauseBtn,
        [&](){
          if (player->isPlaying()){
              Styler::setBtnPause(ui->playPauseBtn);
            }
          else{
              Styler::setBtnPlay(ui->playPauseBtn);
            }
        }
        );
  // nextTrackBtn -> playNextTrack player
  connect(
        ui->nextTrackBtn,
        SIGNAL (clicked(void)),
        this->playerController,
        SLOT (playNextTrack(void))
        );
  // prevTrackBtn -> playPrevTrackBtn player
  connect(
        ui->prevTrackBtn,
        SIGNAL (clicked(void)),
        this->playerController,
        SLOT (playPrevTrack(void))
        );

  // positionChanged player -> setValue slider
  connect(
        playerController,
        SIGNAL(trackPositionChanged(int)),
        timingController,
        SLOT(setValue(int)));
  // durationChanged player -> setMaximum slider

  connect(
        this->playerController,
        &PlayerController::trackDurationChanged,
        ui->slider,
        &QSlider::setMaximum);
  // songChanged player -> setSelected listController
  connect(
        this->playerController,
        &PlayerController::trackChanged,
        this->listController,
        [&](){
        if (player->getPlaylist() != NULL &&
            *(player->getPlaylist()) == *(listController->getPlayList())){
            listController->setSelected(player->getCurrentTrackName());
          }
    });
  // replayTrackBtn -> setLooped player
  connect(
        ui->replayTrackBtn,
        &QPushButton::clicked,
        [&](){
            bool flag = !(player->isLoopedTrack());
            Styler::setBtnRepeatTrack(ui->replayTrackBtn, flag);
            player->setLoopedTrack(flag);
        });
  // randTrackBtn -> setRand player
  connect(
        ui->randTrackBtn,
        &QPushButton::clicked,
        [&](){
      bool flag = !(player->isRandTrack());
      Styler::setBtnRandTrack(ui->randTrackBtn, flag);
      player->setRandTrack(flag);
    });
  // sliderPressed slider -> capture sliderController
  connect(
        ui->slider,
        &QSlider::sliderPressed,
        timingController,
        &TimingController::capture
        );
  // sliderRelease slider -> free sliderController
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
  //
  connect(
        ui->listWidget,
        SIGNAL(customContextMenuRequested(const QPoint &)),
        listController,
        SLOT(ProvideContextMenu(const QPoint &))
        );
  //
  connect(
        ui->volumeSlider,
        &QSlider::valueChanged,
        volumeController,
        &VolumeController::sliderPosChanged
        );
  //
  connect(
        volumeController,
        &VolumeController::volumeSet,
        playerController,
        &PlayerController::setVolume
        );
  // positionChanged player -> update label-timer
  connect(
        this->playerController,
        &PlayerController::trackPositionChanged,
        [&](int time){
      int m, s, ms;
      ms = time % 100;
      s = (time / 1000) % 60;
      m = (time / 60000) % 60;
      QString ttime;
      ttime.sprintf("%.2i:%.2i:%.2i", m, s, ms);
      ui->currentTime->setText(ttime);
    });
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->openGLWidget->setEffect(new OGLTest(ui->openGLWidget->getOGLF()));

    QString type = item->data(Qt::UserRole).toString();
    QString text = item->text();

    if (type == "playlist"){

        QFile temp(fileAssistance->getPlaylistPath(text));
        PlayList* pl = new PlayList(temp);

        listController->clear();
        listController->setPlaylist(pl);
        listController->addGoBack();
        listController->loadTracks();

        if (player->getPlaylist() != NULL && *pl == *(player->getPlaylist())){
            listController->setSelected(player->getCurrentTrackName());
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
        qDebug() << "Track was selected";

        // Configure playlist
        PlayList * temp = listController->getPlayList();
        temp->setSong(text);

        // Change playlist if need
        if (player->getPlaylist() == NULL || *(player->getPlaylist()) != *(temp)){
            qDebug() << "Playlist was updated";
            player->setPlaylist(temp->clone());
          }
        else{
            qDebug() << "Playlist wasn't updated";
            player->getPlaylist()->setSong(text);
          }
        playerController->start();
        //ui->playPauseBtn->setText("Pause");
        ui->slider->setEnabled(true);
        Styler::setBtnPause(ui->playPauseBtn);
      }
}
