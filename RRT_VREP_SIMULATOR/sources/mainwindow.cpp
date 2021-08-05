#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
extern robot Robots;
MainWindow::MainWindow(int clientID,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    renderArea = ui->renderArea;
    rrt = renderArea->rrt;
    simulated = false;
}

/**
 * @brief Start the simulator.
 */
void MainWindow::on_startButton_clicked()
{
	
    if (simulated) {
        ui->statusBox->setText(tr("Please reset!"));
        renderArea->update();
        return;
    }
    simulated = true;
    // get step size and max iterations from GUI.
    rrt->setMaxIterations(ui->maxIterations->text().toInt());
    rrt->setStepSize(ui->stepSize->text().toInt());

    assert(rrt->step_size > 0);
    assert(rrt->max_iter > 0);

    // RRT Algorithm
    for(int i = 0; i < renderArea->rrt->max_iter; i++) {
        Node *q = rrt->getRandomNode();
        if (q) {
            Node *qNearest = rrt->nearest(q->position);
            if (rrt->distance(q->position, qNearest->position) > rrt->step_size) {
                Vector2f newConfig = rrt->newConfig(q, qNearest);
                if (!rrt->obstacles->isSegmentInObstacle(newConfig, qNearest->position)) {
                    Node *qNew = new Node;
                    qNew->position = newConfig;
                    rrt->add(qNearest, qNew);
                }
            }
        }
        if (rrt->reached()) {
            ui->statusBox->setText(tr("Reached Destination!"));
            break;
        }
    }
    Node *q;
    if (rrt->reached()) {
        q = rrt->lastNode;
    }
    else
    {
        // if not reached yet, then shortestPath will start from the closest node to end point.
        q = rrt->nearest(rrt->endPos);
        ui->statusBox->setText(tr("Exceeded max iterations!"));
    }
    // generate shortest path to destination.
    while (q != NULL) {
        rrt->path.push_back(q);
        q = q->parent;
    }
    rrt->genShortPath();
    std::cout<<"guiShortPath Size:	"<<renderArea->rrt->guiShortPath.size()<<"\n";
    std::cout<<"vrepShortPath Size:  "<<renderArea->rrt->vrepShortPath.size()<<"\n";
    int sizeSP = renderArea->rrt->vrepShortPath.size();
    renderArea->update();
    std::cout<<"startpos:\t"<<renderArea->rrt->startPos.x()<<"\t"<<renderArea->rrt->startPos.y()<<std::endl;
    std::cout<<"endpos:\t"<<renderArea->rrt->endPos.x()<<"\t"<<renderArea->rrt->endPos.y()<<std::endl;
    for(int i =  0; i< sizeSP;i++)
    {
        std::cout<<"guiShortPath:\n";
        cout<<renderArea->rrt->guiShortPath[i].x<<"\t"<<renderArea->rrt->guiShortPath[i].y<<"\n";
        std::cout<<"vrepShortPath:\n";
        cout<<renderArea->rrt->vrepShortPath[sizeSP-i-1].x<<"\t"<<renderArea->rrt->vrepShortPath[sizeSP-i-1].y<<"\n";
        move_to_goal(renderArea->rrt->vrepShortPath[i].x,renderArea->rrt->vrepShortPath[i].y,Robots);
        renderArea->update();
    }
    
  	//move_to_goal((renderArea->rrt->endPos.x()/400*5-2.5),(renderArea->rrt->endPos.y()/400*5-2.5),Robots);
    
}

/**
 * @brief Delete all obstacles, nodes and paths from the simulator.
 */
void MainWindow::on_resetButton_clicked()
{
    simulated = false;
    ui->statusBox->setText(tr(""));
    rrt->obstacles->obstacles.clear();
    rrt->obstacles->obstacles.resize(0);
    rrt->deleteNodes(rrt->root);
    rrt->nodes.clear();
    rrt->nodes.resize(0);
    rrt->path.clear();
    rrt->path.resize(0);
    rrt->initialize();
    renderArea->update();
    rrt->guiShortPath.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::move_to_goal(float xg, float yg, robot Robots) {
    float Vmax = 0.1;
    float Wmax = 3.14 / 4;
    float L = 0.1;
    float Kr_V_RL = 0.05;
    float Kr_Prop = 0.02;
    float PI = 3.14;

    datas Data;
    float V, xc, yc, gamma;
    while (1)
    {

        Data = Robots.getData();
        xc = Data.x;
        yc = Data.y;
        gamma = Data.theta;
        cout << "Data:" << xg << "\t" << yg << "\t" << endl;
        if (gamma < 0) {
            gamma += 2.0 * PI;
        }
        float d = float(sqrt((xg - xc) * (xg - xc) + (yg - yc) * (yg - yc)));
        float alpha = float(atan2((yg - yc), (xg - xc)));
        if (alpha < 0) {
            alpha += 2.0 * PI;
        }
        float errorangular = alpha - gamma;
        float w = Wmax * sin(errorangular);
        if (d > Kr_V_RL) {
            V = Vmax;
        }
        else
        {
            V = d * (Vmax / Kr_V_RL);
        }
        //cout << "error:" << d << "\t" << errorangular << endl;
        if (d < 0.05 && d>0) {
            V = 0;
            w = 0;
            Robots.setVel(0, 0);
            cout << "STOP dcmm" << endl;
            break;
        }
        float V_left = (V - (w * L) / 2) * 48;
        float V_right = (V + (w * L) / 2) * 48;
        Robots.setVel(V_right, V_left);
        renderArea->update();
        extApi_sleepMs(5);
    }
}