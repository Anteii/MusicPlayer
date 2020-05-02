#include "listcontroller.h"


ListController::ListController(QObject *parent) : QObject(parent)
{

}

ListController::~ListController()
{
  delete currentPlayList;
}

void ListController::initList(QListWidget * list)
{
  // save pointer to list
  this->list = list;
  // init list of songs
  list->clear();
  isInited = true;
}

void ListController::clear()
{
  if(!isInited) return;
  list->clear();
}

void ListController::addTrack(QString name)
{
  createItem(name, "track", ":/ListItems/resources/icons/listItems/track.png");
}

void ListController::addPlayList(QString name)
{
  createItem(name, "playlist", ":/ListItems/resources/icons/listItems/playlist.png");
}

void ListController::addGoBack()
{
  createItem("", "command_goBack", ":/ListItems/resources/icons/listItems/back.png");
}

void ListController::loadTracks()
{
  if(!isInited || currentPlayList == NULL) return;
  QList<QString> * temp = currentPlayList->getSongList();
  for (int i = 0; i < temp->length(); ++i) {
      addTrack(temp->at(i));
    }
  _whatDisplays = TRACKS;
}

void ListController::loadPlaylists()
{
  if(!isInited) return;
  // Get list of playlists
  QList<QFileInfo> a = (QDir(FileAssistant::getPlaylistsPath())).entryInfoList();
  foreach(QFileInfo pl, a){
      if (pl.fileName() == "." || pl.fileName() == "..") continue;
      addPlayList(pl.baseName());
    }

  _whatDisplays = PLAYLISTS;
}

void ListController::setPlaylist(PlayList * pl)
{
  if (currentPlayList != NULL) delete currentPlayList;
  currentPlayList = pl;
}

void ListController::setLogger(Logger *_logger)
{
  logger = _logger;
}

ListController::contentType ListController::whatDisplays()
{
  return _whatDisplays;
}

void ListController::setSelected(QString name)
{
  if (_whatDisplays != TRACKS) return;
  QList<QString> * temp = currentPlayList->getSongList();
  for (int i = 0; i < temp->length(); ++i) {
      if(temp->at(i) == name){
          list->setCurrentRow(i + 1);
          return;
        }
    }
}

void ListController::ProvideContextMenu(const QPoint &pos)
{
  QPoint item = list->mapToGlobal(pos);
  QMenu submenu;
  submenu.addAction("ADD");
  submenu.addAction("Delete");
  QAction* rightClickItem = submenu.exec(item);
  if (rightClickItem && rightClickItem->text().contains("Delete") )
  {
      //list->takeItem(list->indexAt(pos).row());
  }
}

PlayList *ListController::getPlayList()
{
  return currentPlayList;
}

void ListController::createItem(QString name, QString type, QString icoPath)
{
  if(!isInited) return;
  QListWidgetItem * tempit = new QListWidgetItem;
  //tempit = new QListWidgetItem;
  tempit->setText(name);
  tempit->setData(Qt::UserRole, QVariant(type));
  if (icoPath != 0){
      tempit->setIcon(QIcon(icoPath));
    }
  list->addItem(tempit);
}
