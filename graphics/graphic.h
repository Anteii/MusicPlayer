#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QDebug>
#include <thread>

#include <player_core/player.h>

#include "ogltest.h"

// Baisc class wich will be containing different visualisations
/*!
 * @brief The Graphic class
 *
 * This class present Qt QopenGLWidget and it also implements QOpenGLFunctions_4_3_Core
 */
class Graphic : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
  Q_OBJECT

public:
  //! Constructor
  explicit Graphic(QWidget *parent = nullptr);
  //! Return pointer to QOpenGLFunctions_4_3_Core
  OGLF* getOGLF();
  //! Set visualization (DO NOT DELETE OLD)
  //! @sa Visualization
  void setEffect(Visualization * ef);
  //! Init visualization
  void initEffect();
  //! Set player controller
  void setPlayerController(PlayerController * pctr);
  //! Deinit visualization
  //! @sa Visualization
  void deInitEffect();
  //! Check whether OGL stuff is initted or not
  bool isInited();
  //! Check whether visualization is initted or not
  bool isInitedEffect();
  //! If pass true to this method then visualization wouldn't update
  void setRedFlag(bool);
  //! Return red flag state
  bool getRedFlag();
  //! Is currently updating
  bool isUpdating();
  //! Clear screen
  void clear();
private:
  //! Pointer to Visualization object
  Visualization * effect;
  //! Render text in coordinate (works ocasionally)
  void renderText(double x, double y, double z, const QString & str, const QFont & font, int listBase);
  //! Do some projection stuff
  GLint project(GLdouble objx, GLdouble objy, GLdouble objz,
                const GLdouble model[16], const GLdouble proj[16],
                const GLint viewport[4],
                GLdouble * winx, GLdouble * winy, GLdouble * winz);
  //! Do some geometric stuff
  void transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]);

protected:
  //! @sa Visualization
  void initializeGL() override;
  //! @sa Visualization
  void resizeGL(int h, int w) override;
  //! @sa Visualization
  void paintGL() override;

signals:

private:
  //! Is paintGL now painting
  bool _isUpdating = false;
  //! Pointer to painter to draw some stuff
  QPainter * _painter;
  //! @sa isInited()
  bool _isInited;
  //! @sa isInitedEffect()
  bool _isInitedEffect;
  //! If this flag set with true, then visualization wouldn't update
  bool redFlag = false;
  //! @sa PlayerController
  PlayerController * playerController;
};

#endif // GRAPHIC_H
