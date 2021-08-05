#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RRT_VREP_SIMULATOR.h"

class RRT_VREP_SIMULATOR : public QMainWindow
{
    Q_OBJECT

public:
    RRT_VREP_SIMULATOR(QWidget *parent = Q_NULLPTR);

private:
    Ui::RRT_VREP_SIMULATORClass ui;
};
