#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QOpenGLFunctions_4_3_Core>
#include <controllers/playercontroller.h>
#include <player_core/player.h>
#include "global_types/musicfile.h"
#define OGLF QOpenGLFunctions_4_3_Core

/*!
 * \brief The Visualization class
 *
 * Interface to visualization classes
 */
class Visualization{
public:
  //! In this method should be called all functions to prepare to draw
  //! Load shaders, init buffers etc.
  virtual void init() = 0;
  //! In this method should be called all functions to free resources
  virtual void deInit() = 0;
  //! In this method should be used all functions to draw
  virtual void update() = 0;
  /*!
   * @brief This method should save pointer to playerController class
   *
   * This pointer should be used in order to get track and player data (PCM, time offset)
   **/
  virtual void setPlayerController(PlayerController * ctr) = 0;
};

#endif // VISUALIZATION_H
