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
    qDebug() << "as";
    emit gc->changeVisualization(type);
  }, this, type);
  waitAndGo.detach();
}

void GraphicController::setVisualization(int type)
{

  graphic->deInitEffect();

  if (vis != NULL) delete vis;

  if (!graphic->isInited())
    return delaySet((VisualizationTypes)type);

  switch (type){

    case NONE:
      vis = NULL;
    break;

    case TEST:
      vis = new OGLTest((OGLF*)graphic);
      graphic->setEffect(vis);
      graphic->initEffect();

    break;

  }
  graphic->update();
  current = (VisualizationTypes)type;
}

void GraphicController::setMusicFile(MusicFile * musicFile)
{
  if (this->musicFile != NULL) delete musicFile;
  this->musicFile = musicFile;
  graphic->updateMusicFile(musicFile);
  qDebug() << "musicFile";
}

void GraphicController::initUpdaterThread()
{
  updater = new std::thread([](int gcPtr){
      GraphicController * gc = (GraphicController*)gcPtr;
      Graphic * graphic = gc->graphic;
      while(true){
          while(graphic->isInitedEffect()){
              graphic->update();
              _sleep(68);
            }
          _sleep(17);
        }
    }, (int)this);
  updater->detach();
}
