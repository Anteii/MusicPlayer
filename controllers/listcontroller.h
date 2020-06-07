#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include <QObject>
#include <QListWidget>
#include <QMenu>

#include <vector>

#include "global_types/playlist.h"
#include "static_classes/fileassistant.h"
#include "logger/logger.h"
#include "forms/playlisteditingwindow.h"


/*!
 * @brief The List controller class
 *
 * Controll QListWidget of tracks|playlists
 *
 */
class ListController : public QObject
{
  Q_OBJECT
public:
  //! Contentent type
  enum contentType{
    PLAYLISTS, TRACKS
  };
  //! Constructor
  explicit ListController(QObject *parent = nullptr);
  //! Destructor
  ~ListController();
  //! Set list
  void initList(QListWidget * list);
  //! Add track to list
  void addTrack(QString name);
  //! Add playlist to list
  void addPlayList(QString name);
  //! Add go back button to list
  void addGoBack();
  //! Clear list
  void clear();
  /*!
   * \brief Clear list and call and redraw content
   *
   * Call loadTracks() if displays tracks
   * call loadPlaylists() in opposite case
   *
   */
  void update();
  //! Load tracks from current playlist to list
  void loadTracks();
  //! Load all playlists from playlists directory to list
  void loadPlaylists();
  //! Set playlist (OLD WILL BE DELETED)
  void setPlaylist(PlayList *);
  //! Set logger
  void setLogger(Logger * _logger);
  //! Update list according to playlist
  void updateTrackList(PlayList * pl);
  //! Return content type
  //! @sa contentType
  contentType whatDisplays();
  //! Return current playlist
  PlayList * getPlayList();

public slots:
  void setSelected(const TrackInfo& trackInfo);
  void setSelected(int index);
  void ProvideContextMenu(const QPoint &pos);

private:
  //! Delete selected playlist file
  void deletePlaylist(int ind);
  //! Create playlist creating window
  void addPlaylist();
  //! Create playlist editing window
  void editPlaylist();
  //! Create and add item to list
  void createItem(QString name, QString contentType, QString icoPath = "");
  //! Return index of selected item
  int getSelectedIndex();
  //! Return selected item
  QListWidgetItem * getSelectedItem();

  Logger * logger = NULL;
  bool isInited = false;
  contentType _whatDisplays = PLAYLISTS;
  QListWidget* list = NULL;
  PlayList * currentPlayList = NULL;

};

#endif // LISTCONTROLLER_H
