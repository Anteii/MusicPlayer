#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QDebug>
SettingsWindow::SettingsWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsWindow)
{
  visualizationType = GraphicController::NONE;
  ui->setupUi(this);
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

void SettingsWindow::on_viz_1_clicked()
{
  LOG(Logger::Message, "Choose 'TEST' visualization");
  visualizationType = GraphicController::TEST;
  emit visTypeChanged(visualizationType);
}

void SettingsWindow::on_viz_2_clicked()
{
  LOG(Logger::Message, "Choose 'FIRST' visualization");
  visualizationType = GraphicController::FIRST;
  emit visTypeChanged(visualizationType);
}