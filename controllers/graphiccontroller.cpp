#include "graphiccontroller.h"

GraphicController::GraphicController(QObject *parent) :
  QObject(parent), current(NONE), vis(NULL), graphic(NULL), updater(NULL)
{
  connect(
        this,
        SIGNAL (changeVisualization(int)),
        this,
        SLOT (setVisualization(int))
        );
  initUpdaterThread();
}

void GraphicController::init(Graphic * w)
{
  graphic = w;
  flags._isInited = true;
}

bool GraphicController::isInited()
{
  return flags._isInited;
}

void GraphicController::delaySet(VisualizationTypes type)
{
  std::thread waitAndGo([](GraphicController* gc, VisualizationTypes type){
    // wait till context isn't created
    while(!gc->graphic->isInited()) _sleep(100);
    emit gc->changeVisualization(type);
  }, this, type);
  waitAndGo.detach();
}

void GraphicController::setLogger(Logger *_logger)
{
  logger = _logger;
}

void GraphicController::handleChangedTrack()
{
  LOG(Logger::Message, "Start track changing handling in graphicController");
  LOG(Logger::Message, "Pause graphic updating");
  graphic->setRedFlag(true);
  LOG(Logger::Message, "Wait while graphic widget still updating");
  while(graphic->isUpdating());
  emit readyToChange();
  //trackChanging = true;
}

GraphicController::~GraphicController()
{
  stopUpdating();
  graphic->setRedFlag(true);
  while(graphic->isUpdating());
  flags._shutDown = true;
  while(flags._updaterIsRunning);
  graphic->deInitEffect();
  delete vis;
  delete updater;
}

void GraphicController::startUpdating()
{
  graphic->setRedFlag(false);
  flags._isUpdating = true;
}

void GraphicController::stopUpdating()
{
  flags._isUpdating = false;
  qDebug() << "PUSSY1";
  graphic->setRedFlag(true);
  qDebug() << "PUSSY2";
  while(graphic->isUpdating());
  qDebug() << "PUSSY3";
}

void GraphicController::setVisualization(int type)
{
  stopUpdating();
  _setVisualization(type);
  startUpdating();
}

void GraphicController::setPlayerController(PlayerController *pc)
{
  playerController = pc;
  graphic->setPlayerController(pc);
}

void GraphicController::_setVisualization(int type)
{
  LOG(Logger::Message, "Deiniting current effect");
  graphic->deInitEffect();
  // Exclusively for first launch (PROBABLY)
  if (!graphic->isInited())
    return delaySet((VisualizationTypes)type);

  if (vis != NULL) delete vis;
  LOG(Logger::Message, "Creating new effect and initializing it");
  switch (type){

    case NONE:
      vis = NULL;
      graphic->setRedFlag(true);
      graphic->clear();
      graphic->setRedFlag(false);
      while(graphic->isUpdating());
      graphic->update();
    break;

    case TEST:
      vis = new OGLTest((OGLF*)(this->graphic));
      graphic->setEffect(vis);
      graphic->initEffect();
    break;

    case FIRST:
      vis = new FourierDraw((OGLF*)(this->graphic));
      graphic->setEffect(vis);
      graphic->initEffect();
    break;

  }
  current = (VisualizationTypes)type;
}

void GraphicController::initUpdaterThread()
{
  LOG(Logger::Message, "Start graphic updater thread");
  #define logger_name gc->logger
  updater = new std::thread([](int gcPtr){
      // getting variables
      GraphicController * gc = (GraphicController*)gcPtr;
      Graphic * graphic = gc->graphic;
      auto flags = &gc->flags;

      flags->_updaterIsRunning = true;
      // wait till not initialized
      while(!flags->_isInited) _sleep(100);
      // starting updating loop
      while(true){
           // updating
           while (flags->_isUpdating){
               graphic->update();
               _sleep(68);
             }
           if (flags->_shutDown) break;
           _sleep(60);
        }
      flags->_updaterIsRunning = false;
    }, (int)this);
  #define logger_name logger

  updater->detach();
}
