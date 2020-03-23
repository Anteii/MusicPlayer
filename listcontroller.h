#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include <QObject>
#include <QListWidget>
#include <QMenu>

#include "playlist.h"
#include "fileassistant.h"


class ListController : public QObject
{
  Q_OBJECT
public:
  enum contentType{
    PLAYLISTS, TRACKS
  };
  explicit ListController(QObject *parent = nullptr);

  void initList(QListWidget * list);

  void addTrack(QString name);
  void addPlayList(QString name);
  void addGoBack();

  void clear();

  void loadTracks();
  void loadPlaylists();

  void setPlaylist(PlayList *);

  contentType whatDisplays();
  PlayList * getPlayList();

private:

  void createItem(QString name, QString contentType, QString icoPath = "");

  bool isInited = false;
  contentType _whatDisplays = PLAYLISTS;

  QListWidget* list = NULL;
  PlayList * currentPlayList = NULL;

signals:

public slots:
  void setSelected(QString name);
  void ProvideContextMenu(const QPoint &pos);
};

#endif // LISTCONTROLLER_H
