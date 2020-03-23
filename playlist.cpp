#include "playlist.h"


PlayList::PlayList(QDir & dir) : PlayList()
{
  QStringList names = dir.entryList();
  foreach(QString str, names){
      if (str.compare("..") != 0 && str.compare(".") != 0)
        list->push_back(str);
    }
}

PlayList::PlayList(QFile &  file) : PlayList()
{
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QTextStream st(&file);
  st.setCodec("UTF-8");
  QString line;

  while (st.readLineInto(&line)) {
      list->push_back(QString(line));
  }

  file.close();

}

PlayList::PlayList()
{
  list = new QList<QString>;
  currentSong = 0;
}

PlayList::~PlayList()
{
  delete list;
}

QString PlayList::getNextSong()
{
  if (currentSong == list->length() - 1) return "";
  return list->at(++currentSong);
}

QString PlayList::getPrevSong()
{
  if(currentSong == 0) return "";
  return list->at(--currentSong);
}

QString PlayList::getSong()
{
  return list->at(currentSong);
}

void PlayList::toFirstSong()
{
  currentSong = 0;
}

void PlayList::toLastSong()
{
  currentSong = list->length() - 1;
}

void PlayList::setPosition(int index)
{
  if (index >= 0 && index < list->length()){
      currentSong = index;
    }
}

void PlayList::setSong(QString name)
{
  for(int i = 0; i < list->length(); ++i){
      if (list->at(i) == name){
          currentSong = i;
          break;
        }
    }
  qDebug() << "Song number " << currentSong << "was set";
}

int PlayList::getSongCount()
{
  return list->length();
}

PlayList *PlayList::clone()
{
  PlayList * temp = new PlayList;
  for(int i = 0; i < list->length(); ++i){
      temp->list->push_back(list->at(i));
    }
  temp->currentSong = currentSong;
  return temp;
}

QList<QString> *PlayList::getSongList()
{
  return list;
}

bool PlayList::operator==(const PlayList & pl) const
{
  if (list->length() != pl.list->length()) return false;
  for(int i = 0; i < list->length(); ++i)
      if(list->at(i) != pl.list->at(i))
        return false;
  return  true;
}

bool PlayList::operator!=(const PlayList & pl) const
{
  if (list->length() != pl.list->length()) return true;
  for(int i = 0; i < list->length(); ++i)
      if(list->at(i) != pl.list->at(i))
        return true;
  return  false;
}
