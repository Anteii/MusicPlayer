#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include <QObject>
#include <QListWidget>
#include <QMenu>

#include "global_types/playlist.h"
#include "static_classes/fileassistant.h"
#include "logger/logger.h"
#include <vector>
class ListController : public QObject
{
  Q_OBJECT
public:
  enum contentType{
    PLAYLISTS, TRACKS
  };
  explicit ListController(QObject *parent = nullptr);
  ~ListController();
  void initList(QListWidget * list);

  void addTrack(QString name);
  void addPlayList(QString name);
  void addGoBack();

  void clear();

  void loadTracks();
  void loadPlaylists();

  void setPlaylist(PlayList *);
  void setLogger(Logger * _logger);
  contentType whatDisplays();
  PlayList * getPlayList();

private:

  void createItem(QString name, QString contentType, QString icoPath = "");
  Logger * logger = NULL;
  bool isInited = false;
  contentType _whatDisplays = PLAYLISTS;

  QListWidget* list = NULL;
  PlayList * currentPlayList = NULL;

signals:

public slots:
  void setSelected(const TrackInfo& trackInfo);
  void setSelected(int index);
  void ProvideContextMenu(const QPoint &pos);
};

#endif // LISTCONTROLLER_H
