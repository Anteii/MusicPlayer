#include "playlist.h"
namespace fs = std::experimental::filesystem;

std::string PlayList::playlistsDirectory = "playlists";
std::string PlayList::defaultTrackDirectory = "music";
std::string getFileNameFromPath(const std::string& path){
  std::string name;
  int sPos;
  if (path.find_last_of("\\") != std::string::npos){
      sPos = path.find_last_of("\\");
      name = path.substr(sPos + 1, path.size()).c_str();
    }
  else{
      sPos = path.find_last_of("/");
      name = path.substr(sPos + 1, path.size()).c_str();
    }
  return name;
}
PlayList::PlayList(const std::string& path) : PlayList()
{
  const fs::path fsPath(path); // Constructing the path from a string is possible.
  std::error_code ec; // For using the non-throwing overloads of functions below.
  std::string temp;
  int namePos;
  if (fs::is_directory(path, ec))
  {
     playlistName = getFileNameFromPath(path);
     for (const auto & entry : fs::directory_iterator(path)){
         temp = getFileNameFromPath(entry.path().string());
         if (isSupportedFile(temp)){
            list.push_back(temp);
           }
       }
  }
  else if (fs::is_regular_file(path, ec))
  {
      namePos = path.find_last_of('.');
      playlistName = getFileNameFromPath(path.substr(0, namePos - 1));
      std::ifstream in(path);
      while(!in.eof()){
          std::getline(in, temp);
          if (isSupportedFile(temp)){
              list.push_back(temp);
            }
        }
      in.close();
  }
  else{
      // Handling
  }
  qDebug() << playlistName.c_str();
}

PlayList::PlayList(){
  currentSong = 0;
  playlistName = "";
  trackDirectory = defaultTrackDirectory;
}

bool PlayList::isSupportedFile(const std::string &fileName)
{
  int ext_pos = fileName.find_last_of('.');
  std::string ext = fileName.substr(ext_pos + 1, fileName.size() - 1);
  for(auto supF : supportedFormats){
      if (supF == ext){
          return true;
        }
    }
  return false;
}

PlayList::~PlayList()
{
}

std::string PlayList::getName()
{
  return playlistName;
}

// Iterating from currentSong to list.size()-1 position, deleting invalid tracks.
// Then, if need, go backward also deleting invalid elements.
std::string PlayList::getNextSong()
{
  if (currentSong == list.size() - 1) return "";
  std::ifstream checker;
  while (++currentSong < list.size()){
      checker.open(trackDirectory + "//" + getCurrentSong());
      if (checker.good()) break;
      list.erase(list.begin() + currentSong);
      --currentSong;
      checker.close();
    }
  if (currentSong == -1){
      while(--currentSong > -1){
          checker.open(trackDirectory + "//" + getCurrentSong());
          if (checker.good()) break;
          list.erase(list.begin() + currentSong);
          ++currentSong;
          checker.close();
        }
      if (list.size() == 0){
          return "";
        }
    }
  return getCurrentSong();
}

// Iterating from currentSong to 0 position, deleting invalid tracks. Then, if need,
// go forward also deleting invalid elements.
std::string PlayList::getPrevSong()
{
  if(currentSong == 0) return "";
  std::ifstream checker;
  while (--currentSong > -1){
      checker.open(trackDirectory + "//" + getCurrentSong());
      if (checker.good()) break;
      list.erase(list.begin() + currentSong);
      ++currentSong;
      checker.close();
    }
  if (currentSong == -1){
      while(++currentSong < list.size()){
          checker.open(trackDirectory + "//" + getCurrentSong());
          if (checker.good()) break;
          list.erase(list.begin() + currentSong);
          --currentSong;
          checker.close();
        }
      if (list.size() == 0){
          return "";
        }
    }
  return getCurrentSong();
}

std::string PlayList::getCurrentSong()
{
  return list.at(currentSong);
}

void PlayList::toFirstSong()
{
  currentSong = 0;
}

void PlayList::toLastSong()
{
  currentSong = list.size() - 1;
}

void PlayList::setPosition(int index)
{
  if (index >= 0 && index < list.size()){
      currentSong = index;
    }
}

void PlayList::setSong(const std::string& name)
{
  for(int i = 0; i < list.size(); ++i){
      if (list.at(i) == name){
          currentSong = i;
          break;
        }
    }
}

int PlayList::getSongCount()
{
  return list.size();
}

PlayList *PlayList::clone()
{
  PlayList * temp = new PlayList;
  for(int i = 0; i < list.size(); ++i){
      temp->list.push_back(list.at(i));
    }
  temp->playlistName = playlistName;
  temp->trackDirectory = trackDirectory;
  temp->currentSong = currentSong;
  return temp;
}

std::vector<std::string> *PlayList::getSongList()
{
  return &list;
}

bool PlayList::operator==(const PlayList & pl) const
{
  if (list.size() != pl.list.size()) return false;
  for(int i = 0; i < list.size(); ++i)
      if(list.at(i) != pl.list.at(i))
        return false;
  return  true;
}

bool PlayList::operator!=(const PlayList & pl) const
{
  return !(*this == pl);
}
