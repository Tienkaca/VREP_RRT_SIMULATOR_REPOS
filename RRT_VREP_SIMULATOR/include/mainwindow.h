#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "renderarea.h"
#include "rrt.h"
#include "robot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(int clientID,QWidget *parent = 0);
    
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    RenderArea *renderArea;
    RRT *rrt;
    QTimer *m_timer;
    bool simulated;
private slots:
    void move_to_goal();
    void on_startButton_clicked();
    void on_resetButton_clicked();
};

#endif // MAINWINDOW_H
