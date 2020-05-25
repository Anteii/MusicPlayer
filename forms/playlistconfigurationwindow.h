#ifndef PLAYLISTCONFIGURATIONWINDOW_H
#define PLAYLISTCONFIGURATIONWINDOW_H

#include <QDialog>
#include <QCheckBox>
#include "global_types/playlist.h"


namespace Ui {
  class PlaylistConfigurationWindow;
}

class PlaylistConfigurationWindow : public QDialog
{
  Q_OBJECT

public:
  explicit PlaylistConfigurationWindow(QWidget *parent = nullptr);
  ~PlaylistConfigurationWindow();
  void addTrack(TrackInfo track, bool checked = false);
  void setPlaylist(PlayList * pl = NULL);
  void setAllChecked();
protected:
  void accept() override;
private:
  PlayList * pl;
  QList<TrackInfo> list;
  Ui::PlaylistConfigurationWindow *ui;
};

#endif // PLAYLISTCONFIGURATIONWINDOW_H
