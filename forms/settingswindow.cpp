#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QDebug>
SettingsWindow::SettingsWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsWindow)
{
  ui->setupUi(this);
  visualizationType = GraphicController::NONE;
  this->setWindowIcon(QIcon(":/Windows/resources/icons/windows/settingsWindow.png"));
  this->setWindowTitle("Settings");
  setFixedSize(size());
}

void SettingsWindow::setLogger(Logger *_logger)
{
  logger= _logger;
}

SettingsWindow::~SettingsWindow()
{
  delete ui;
}

void SettingsWindow::on_viz_0_clicked()
{
  LOG(Logger::Message, "Choose 'NONE' visualization");
  visualizationType = GraphicController::NONE;
  emit visTypeChanged(visualizationType);
}

void SettingsWindow::on_viz_2_clicked()
{
  LOG(Logger::Message, "Choose 'FFT' visualization");
  visualizationType = GraphicController::FFT;
  emit visTypeChanged(visualizationType);
}

void SettingsWindow::on_radioButton_clicked()
{
    LOG(Logger::Message, "Choose 'DFT' visualization");
    visualizationType = GraphicController::DFT;
    emit visTypeChanged(visualizationType);
}
