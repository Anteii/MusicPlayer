#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QOpenGLFunctions_4_3_Core>
#include <player_core/player.h>
#include "global_types/musicfile.h"
#define OGLF QOpenGLFunctions_4_3_Core


class Visualization : public OGLF{
public:
  explicit Visualization(OGLF * f) : OGLF(*f){};
  virtual void init() = 0;
  virtual void deInit() = 0;
  virtual void update() = 0;
  virtual void setMusicFile(MusicFile*) = 0;
  virtual void setPlayer(Player *pl) = 0;
};

#endif // VISUALIZATION_H
