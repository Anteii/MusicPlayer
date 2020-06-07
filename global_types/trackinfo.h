#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <string>

/*!
 * \brief The track information class
 *
 * Keep some track data and provide it to other components
 */
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
