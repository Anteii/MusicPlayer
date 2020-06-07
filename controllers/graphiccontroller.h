#ifndef GRAPHICCONTROLLER_H
#define GRAPHICCONTROLLER_H

#include <QOpenGLWidget>
#include <QString>

#include <thread>
#include "global_types/musicfile.h"
#include "graphics/graphic.h"
#include "graphics/Visualization.h"
#include "graphics/ogltest.h"
#include "graphics/FourierDraw.h"
#include "logger/logger.h"

// Define logger nam
#define logger_name logger
/*!
 * @brief The GraphicController class
 *
 * This class provide some controll funciton
 *
 * It used to controll current visualization, updating, etc.
 */
class GraphicController : public QObject
{
  Q_OBJECT
public:
  enum VisualizationTypes{
    NONE, FIRST, FOURIER
  };
  //! Constructor
  explicit GraphicController(QObject *parent = nullptr);
  //! Set Graphic widget to this controller
  void init(Graphic *);
  //! Check whether Graphic widget was set or not
  //! @sa init()
  bool isInited();
  //! Whait till it's not inited
  //! @sa init(), isInited()
  void delaySet(VisualizationTypes type);
  //! Set logger
  void setLogger(Logger * _logger);
  //! Destructor
  /*!
   * Stop updating, deinit visualization, delete visualization
   */
  ~GraphicController();
public slots:
  //! Start or resume updating visualization
  void startUpdating();
  /*!
   * \brief Stop updating visualization
   *
   * This function stop updating visualization
   */
  void stopUpdating();
  //! Public method
  //! @sa _setVisualization()
  void setVisualization(int);
  //! Set player controller to itself and to Graphic widget (if it's inited)
  void setPlayerController(PlayerController * pc);
signals:
  void changeVisualization(int);
  void readyToChange();
private:
  // Struct for keeping inner state
  struct {
    //! Is visualization updating now
    bool _isUpdating = false;
    //! Is Graphic widget was set
    bool _isInited = false;
    //! Flag for some thread operations
    bool _synchronizedFlag = false;
    //! Is we going to stop updating thread
    bool _shutDown = false;
    //! Is updating thread still going
    bool _updaterIsRunning = false;
  } flags;
  /*!
   * \brief Set visualization
   *
   * Set visualization, if one was already set, then deinit it and delete
   *
   * \param type type of visualization
   *
   * @sa VisualizationTypes
   */
  void _setVisualization(int type);
  //! Initialize updating thread
  void initUpdaterThread();

  Logger * logger = NULL;
  Visualization * vis = NULL;
  Graphic * graphic = NULL;
  std::thread * updater = NULL;
  PlayerController * playerController = NULL;

  VisualizationTypes current;
};


#endif // GRAPHICCONTROLLER_H
