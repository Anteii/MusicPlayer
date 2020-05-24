#include "trackinfo.h"

std::string getFileName(const std::string &path)
{
  int namePos;
  int nameEnd = path.size();
  if ((namePos = path.find_last_of("/")) != std::string::npos);
  else if ((namePos = path.find_last_of("\\")) != std::string::npos);
  else namePos = 0;
  if (path.find_last_of(".") != std::string::npos && path.find_last_of(".") > namePos)
    nameEnd = path.find_last_of(".");
  return path.substr(namePos + 1, nameEnd - namePos - 1);
}

std::string getFileExt(const std::string &path)
{
  int ext_pos = path.find_last_of('.');
  std::string ext = path.substr(ext_pos + 1, path.size() - 1).c_str();
  return ext;
}

std::string TrackInfo::getPath() const
{
  return path;
}

bool TrackInfo::operator==(const TrackInfo &temp) const
{
  return temp.path == path;
}

bool TrackInfo::operator!=(const TrackInfo &temp) const
{
  return temp.path != path;
}

TrackInfo::TrackInfo(const std::string& _path) : path(_path)
{
  name = getFileName(path);
  ext = getFileExt(path);
}

TrackInfo::TrackInfo(const TrackInfo &it)
{
  path = it.path;
  name = it.name;
  ext = it.ext;
}

std::string TrackInfo::getExt() const
{
  return ext;
}

std::string TrackInfo::getName() const
{
  return name;
}
