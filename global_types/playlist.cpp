#include "playlist.h"


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
            list.push_back(path + "//" + temp);
           }
       }
  }
  else if (fs::is_regular_file(path, ec))
  {
      namePos = path.find_last_of('.');
      playlistName = getFileNameFromPath(path.substr(0, namePos));
      std::ifstream in(path);
      while(!in.eof()){
          std::getline(in, temp);
          if (isSupportedFile(temp)){
              list.push_back(defaultTrackDirectory + "//" + temp);
            }
        }
      in.close();
  }
  else{
      // Handling
  }
}

PlayList::PlayList(){
  currentIndex = 0;
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

void PlayList::setName(const std::string & _name)
{
  playlistName = _name;
}

// Iterating from currentSong to list.size()-1 position, deleting invalid tracks.
// Then, if need, go backward also deleting invalid elements.
TrackInfo PlayList::getNextSong()
{
  if (currentIndex == list.size() - 1)
    throw std::out_of_range(std::string("Playlist out of range"));
  ++currentIndex;
  return getCurrentSong();
}

// Iterating from currentSong to 0 position, deleting invalid tracks. Then, if need,
// go forward also deleting invalid elements.
TrackInfo PlayList::getPrevSong()
{
  if(currentIndex == 0)
    throw std::out_of_range(std::string("Playlist out of range"));
  --currentIndex;
  return getCurrentSong();
}

TrackInfo PlayList::getCurrentSong()
{
  std::ifstream checker;
  checker.open(list.at(currentIndex).getPath());
  if (checker.good()){
      return list.at(currentIndex);
    }
  throw std::logic_error(
        list.at(currentIndex).getName() + "." +list.at(currentIndex).getExt() + " " +
        "is invalid file.");
}

void PlayList::toFirstSong()
{
  currentIndex = 0;
}

void PlayList::toLastSong()
{
  currentIndex = list.size() - 1;
}

void PlayList::setPosition(int index)
{
  if (index >= 0 && index < list.size()){
      currentIndex = index;
    }
}

void PlayList::setSong(const TrackInfo& track)
{
  for(int i = 0; i < list.size(); ++i){
      if (list.at(i) == track){
          currentIndex = i;
          break;
        }
    }
}

int PlayList::size()
{
  return list.size();
}

bool PlayList::contains(const TrackInfo &info)
{
  for(int i = 0; i < list.size(); ++i){
      if (list.at(i) == info){
          return true;
        }
    }
  return false;
}

PlayList *PlayList::clone()
{
  PlayList * temp = new PlayList;
  for(int i = 0; i < list.size(); ++i){
      temp->list.push_back(list.at(i));
    }
  temp->playlistName = playlistName;
  temp->trackDirectory = trackDirectory;
  temp->currentIndex = currentIndex;
  return temp;
}

std::vector<TrackInfo> *PlayList::getSongList()
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
