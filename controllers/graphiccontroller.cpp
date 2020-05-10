#include "graphiccontroller.h"

GraphicController::GraphicController(QObject *parent) :
  QObject(parent), _isInited(false), current(NONE), vis(NULL), graphic(NULL), updater(NULL)
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
  _isInited = true;
}

bool GraphicController::isInited()
{
  return _isInited;
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
  trackChanging = true;
}

GraphicController::~GraphicController()
{
  _shutDown = true;
  while(_updaterIsRunning);
  graphic->setRedFlag(true);
  while(graphic->isUpdating());
  graphic->deInitEffect();
  delete vis;
  delete updater;
}

void GraphicController::setVisualization(int type)
{
  LOG(Logger::Message, "Start visualization changing");
  nextVisType = type;
  LOG(Logger::Message, "Wait for graphic updater thread");
  while(!synchronizedFlag);
  _setVisualization(nextVisType);
  LOG(Logger::Message, "New effect was create and initialize");
  nextVisType = -1;
  graphic->setRedFlag(false);
  synchronizedFlag = false;
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
      GraphicController * gc = (GraphicController*)gcPtr;
      Graphic * graphic = gc->graphic;
      while(true){
          while(graphic->isInitedEffect() && gc->nextVisType == -1 && !(gc->trackChanging) && !gc->_shutDown){
              graphic->update();
              _sleep(50);
            }
          if (gc->trackChanging){
              LOG(Logger::Message, "Wait while player still playing");
              while(!gc->playerController->isPlaying());
              LOG(Logger::Message, "Resume graphic updating");
              gc->trackChanging = false;
              graphic->setRedFlag(false);
              LOG(Logger::Message, "End track changing handling in graphic updater thread");
            }
          if (gc->nextVisType != -1){
              LOG(Logger::Message, "Visualization changing in graphic updater thread");
              graphic->setRedFlag(true);
              LOG(Logger::Message, "Wait while graphic widget still updating");
              while(graphic->isUpdating()){
                  _sleep(10);
                }
              LOG(Logger::Message, "Wait for main thread set up new visualization");
              gc->synchronizedFlag = true;
              while(gc->synchronizedFlag);
              LOG(Logger::Message, "End visualization changing");
            }
          if (gc->_shutDown){
              break;
            }
          _sleep(68);
        }
      gc->_updaterIsRunning = false;
    }, (int)this);
  #define logger_name logger

  updater->detach();
}
