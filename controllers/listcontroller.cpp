#include "listcontroller.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>


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

void ListController::update()
{
  if (whatDisplays() == TRACKS) {
      clear();
      addGoBack();
      loadTracks();
    }
  else if (whatDisplays() == PLAYLISTS) {
      clear();
      loadPlaylists();
    }
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
  std::vector<TrackInfo> * temp = currentPlayList->getSongList();
  for (int i = 0; i < temp->size(); ++i) {
      addTrack(QString(temp->at(i).getName().c_str()));
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

void ListController::updateTrackList(PlayList *pl)
{
  setPlaylist(pl->clone());
  clear();
  addGoBack();
  loadTracks();
  setSelected(pl->getPosition());
}

ListController::contentType ListController::whatDisplays()
{
  return _whatDisplays;
}

void ListController::setSelected(const TrackInfo& trackInfo)
{
  if (_whatDisplays != TRACKS) return;
  std::vector<TrackInfo> * temp = currentPlayList->getSongList();
  for (int i = 0; i < temp->size(); ++i) {
      if(temp->at(i) == trackInfo){
          list->setCurrentRow(i + 1);
          return;
        }
    }
}

void ListController::setSelected(int index)
{
  if (index < 0 || index > list->count()) return;
  list->setCurrentRow(index + 1);
}

void ListController::ProvideContextMenu(const QPoint &pos)
{
  if (whatDisplays() == TRACKS) return;
  QPoint item = list->mapToGlobal(pos);
  QMenu submenu;
  if (getSelectedIndex() != -1){
      submenu.addAction("edit..");
      submenu.addAction("delete..");
    }
  else{
      submenu.addAction("add..");
      submenu.addAction("update");
    }
  QAction* rightClickItem = submenu.exec(item);
  if (rightClickItem && rightClickItem->text().contains("delete..") )
  {
        if (whatDisplays() == PLAYLISTS) {
          deletePlaylist(getSelectedIndex());
        }
  }
  else if (rightClickItem && rightClickItem->text().contains("add..") ){
        short int* arr;if (whatDisplays() == PLAYLISTS) {
          addPlaylist();
        }
    }
  else if (rightClickItem && rightClickItem->text().contains("update") ){
      update();
    }
  else if (rightClickItem && rightClickItem->text().contains("edit..") ){
      if (whatDisplays() == PLAYLISTS) {
          editPlaylist();
        }
    }
}

PlayList *ListController::getPlayList()
{
  return currentPlayList;
}

void ListController::deletePlaylist(int ind)
{
    QListWidgetItem* item = list->item(ind);
    PlayList::deletePlaylistFile(item->text().toStdString());
    clear();
    loadPlaylists();
}

void ListController::addPlaylist()
{
  PlaylistEditingWindow win;
  win.setModal(true);
  win.setPlaylist();
  //QString fileName = QFileDialog::getOpenFileName( NULL,
  //tr("Open Playlist file"), "/home/", tr("Playlist Files (*.txt)"));
  win.exec();
  update();
}

void ListController::editPlaylist()
{
  PlayList * pl = new PlayList(
        FileAssistant::getPlaylistPath(
          getSelectedItem()->text()).toStdString()
          );
  PlaylistEditingWindow win;
  win.setModal(true);
  win.setPlaylist(pl);
  //QString fileName = QFileDialog::getOpenFileName( NULL,
  //tr("Open Playlist file"), "/home/", tr("Playlist Files (*.txt)"));
  win.exec();
  qDebug() << "Changed";
  update();
}

void ListController::createItem(QString name, QString type, QString icoPath)
{
  if(!isInited) return;
  QListWidgetItem * tempit = new QListWidgetItem;
  tempit->setText(name);
  tempit->setData(Qt::UserRole, QVariant(type));
  if (icoPath != 0){
      tempit->setIcon(QIcon(icoPath));
    }
  list->addItem(tempit);
  if (type.indexOf("command") == -1){
    }
}

int ListController::getSelectedIndex()
{
  for(int i = 0; i < list->count(); ++i){
      if (list->item(i)->isSelected()) return i;
    }
  return -1;
}

QListWidgetItem *ListController::getSelectedItem()
{
  auto t = list->selectedItems();
  if (t.size() == 0) return NULL;
  return t.at(0);
}
