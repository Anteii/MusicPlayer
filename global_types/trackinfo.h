#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <string>

class TrackInfo
{
private:
  std::string name;
  std::string ext;
  std::string path;
public:
  TrackInfo(const std::string& _path);
  TrackInfo(const TrackInfo& it);
  std::string getName() const;
  std::string getExt() const;
  std::string getPath() const;

  bool operator==(const TrackInfo& temp) const;
  bool operator!=(const TrackInfo& temp) const;
};

#endif // TRACKINFO_H
