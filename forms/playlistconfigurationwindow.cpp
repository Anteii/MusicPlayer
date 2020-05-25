#include "playlistconfigurationwindow.h"
#include "ui_playlistconfigurationwindow.h"

#include <QMessageBox>

PlaylistConfigurationWindow::PlaylistConfigurationWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PlaylistConfigurationWindow)
{
  ui->setupUi(this);
}

PlaylistConfigurationWindow::~PlaylistConfigurationWindow()
{
  delete ui;
}

void PlaylistConfigurationWindow::addTrack(TrackInfo track, bool checked)
{
  QListWidgetItem *item = new QListWidgetItem(
        QString((track.getName() + "." + track.getExt()).c_str()), ui->trackList);
  item->setFlags(item->flags() | (Qt::ItemIsUserCheckable));
  item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
  ui->trackList->addItem(item);
}

void PlaylistConfigurationWindow::setPlaylist(PlayList * _pl)
{
  pl = _pl;
  PlayList * alls = PlayList::getBaseTrackPlaylist();
  for(int i = 0; i < alls->size(); ++i){
      try {
        bool flag = false;
        TrackInfo t1 = alls->getCurrentSong();
        if (pl != NULL){
          flag = pl->contains(t1);
        }
        addTrack(t1, flag);
      } catch (std::logic_error err) {
        continue;
      }
      alls->setPosition(alls->getPosition() + 1);
    }
  ui->playlistName->setPlaceholderText("Playlist name");
  if (pl != NULL){
      ui->playlistName->setText(QString(pl->getName().c_str()));
    }
}

void PlaylistConfigurationWindow::setAllChecked()
{
  for(int i = 0; i < ui->trackList->count(); ++i){
      ui->trackList->item(i)->setCheckState(Qt::Checked);
    }
}

void PlaylistConfigurationWindow::accept()
{
  if (ui->playlistName->text() == "") {
      QMessageBox::warning(this, "Be carefull!", "Playlist name field is empty");
      return;
    }
  pl->setName(ui->playlistName->text().toStdString());
  QString path = (PlayList::getPlaylistsDirectory() + "//" + pl->getName() + ".txt").c_str();
  qDebug() << path;
  QList<QString> temp;
  for(int i = 0; i < ui->trackList->count(); ++i){
      auto item = ui->trackList->item(i);
      if (item->checkState() == Qt::Checked)
        temp.append(QString(item->text()));
        qDebug() << item->text();
    }
  if (temp.size() == 0){
      QMessageBox::warning(this, "Be carefull!", "Can't create empty playlist");
      return;
    }
  FileAssistant::WriteFile(path, &temp);
  QDialog::accept();
}
