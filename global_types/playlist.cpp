#include "playlist.h"


PlayList::PlayList(QDir & dir) : PlayList()
{
  QStringList names = dir.entryList();
  QRegExp rx(getREPFileName());
  foreach(QString str, names){
      if (rx.exactMatch(str) && !list->contains(str)){
        list->push_back(str);
      }
    }
}

PlayList::PlayList(QFile &  file) : PlayList()
{
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QTextStream st(&file);
  st.setCodec("UTF-8");
  QString str;
  QRegExp rx(getREPFileName());
  while (st.readLineInto(&str)) {
      if (rx.exactMatch(str) && !list->contains(str)){
        list->push_back(str);
      }
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
  QString songName = list->at(++currentSong);
  if (FileAssistant::checkIfFileExist("music//" + songName))
    return songName;
  else
    return "";
}

QString PlayList::getPrevSong()
{
  if(currentSong == 0) return "";
  QString songName = list->at(--currentSong);
  if (FileAssistant::checkIfFileExist("music//" + songName))
    return songName;
  else
    return "";
}

QString PlayList::getSong()
{
  QString songName = list->at(currentSong);
  if (FileAssistant::checkIfFileExist("music//" + songName))
    return songName;
  else
    return "";
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

QString PlayList::getREPFileName()
{
  QString formatsGroup("");
  for (QString format : supportedFormats){
      formatsGroup.append(format).append("|");
    }
  formatsGroup.remove(formatsGroup.lastIndexOf('|'), 1);
  if (formatsGroup.at(formatsGroup.length() - 1) == "|"){
      formatsGroup.replace(formatsGroup.length() - 1, 1, "");
    }
  return QString(".+\.(").append(formatsGroup).append(")");
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
