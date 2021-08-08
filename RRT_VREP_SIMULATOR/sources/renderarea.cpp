#include "renderarea.h"
#include <queue>
#include <QTimer>
#include <iostream>
#include <math.h>
#include <vector>
#include "object.h"
#include "datatrans.h"
#include "robot.h"
extern "C" {
#include "extApi.h"
}

extern int clientID;
extern robot Robots;
extern datas sts;
extern object obs[NUMBER_OBS];

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    scribbling = false;
    rrt = new RRT;
}

/**
 * @brief Draw the world.
 * @param painter
 */
void RenderArea::drawField(QPainter &painter)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRect field;
    field.setTopLeft(QPoint(this->x(), this->y()));
    field.setBottomRight(QPoint(this->width()-1, this->height()-1));
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::green));
    painter.drawRect(field);
    painter.restore();
}

/**
 * @brief Draw the start position of the bot.
 * @param painter
 */
void RenderArea::drawStartPos(QPainter &painter)
{
    //start my code 
    sts = Robots.getData();
    rrt->startPos.x() = round((sts.x + 2.5)/5*400);
    rrt->startPos.y() = round((sts.y + 2.5)/5*400);
    
    //end my code

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(rrt->startPos.x() - BOT_RADIUS, rrt->startPos.y()  - BOT_RADIUS, 2 * BOT_RADIUS, 2 * BOT_RADIUS);
    painter.restore();
}

/**
 * @brief Draw the end point.
 * @param painter
 */
void RenderArea::drawEndPos(QPainter &painter)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::blue));
    painter.drawEllipse(rrt->endPos.x() - BOT_RADIUS, rrt->endPos.y() - BOT_RADIUS, 2 * BOT_RADIUS, 2 * BOT_RADIUS);
    painter.restore();
    std::cout<<"drawEndPos\n";
}

/**
 * @brief Draw all the rectangular obstacles.
 * @param painter
 */
void RenderArea::drawObstacles(QPainter &painter)
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::black));
    pair<Vector2f, Vector2f> obstacle;
    for(int i = 0; i < NUMBER_OBS; i++) {
        //obstacle = rrt->obstacles->obstacles[i];
        //QPoint topLeft(obstacle.first.x() + BOT_CLEARANCE, obstacle.first.y() + BOT_CLEARANCE);
        //QPoint bottomRight(obstacle.second.x() - BOT_CLEARANCE, obstacle.second.y() - BOT_CLEARANCE);
        obs[i].getData();
        int x_Obs = Vrep2Qt_dataTransform(obs[i].position[0]);
        int y_Obs = Vrep2Qt_dataTransform(obs[i].position[1]);
        int obs_R = (int)(OBS_RADIUS*RD_AREA_SIZE/5);
        rrt->obstacles->addObstacle(Vector2f(x_Obs - obs_R , y_Obs - obs_R), Vector2f(x_Obs + obs_R , y_Obs + obs_R));
        //std::cout << x_Obs <<"  "<< y_Obs <<"   " << obs_R<<std::endl;
        QPoint topLeft(x_Obs - obs_R , y_Obs - obs_R );
        QPoint bottomRight(x_Obs + obs_R ,  y_Obs + obs_R );
        QRect rect(topLeft, bottomRight);
        painter.drawRect(rect);
    }
    painter.restore();
}

/**
 * @brief Draw all the nodes generated in the RRT algorithm.
 * @param painter
 */
void RenderArea::drawNodes(QPainter &painter)
{

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::black));
    Vector2f pos;
    for(int i = 0; i < (int)rrt->nodes.size(); i++) {
        for(int j = 0; j < (int)rrt->nodes[i]->children.size(); j++) {
            pos = rrt->nodes[i]->children[j]->position;
            painter.drawEllipse(pos.x()-1.5, pos.y()-1.5, 3, 3);
        }
        pos = rrt->nodes[i]->position;
        painter.drawEllipse(pos.x() - NODE_RADIUS, pos.y() - NODE_RADIUS, 2 * NODE_RADIUS, 2 * NODE_RADIUS);
    }
    painter.setPen(Qt::red);
    painter.setBrush(QBrush(Qt::red));

    // if a path exists, draw it.
    if (rrt->path.size())
    {
        for (int i = 0; i < (int)rrt->path.size() - 1; i++) {
            QPointF p1(rrt->path[i]->position.x(), rrt->path[i]->position.y());
            QPointF p2(rrt->path[i + 1]->position.x(), rrt->path[i + 1]->position.y());
            painter.drawLine(p1, p2);
        }
    }
    // draw rush path
    painter.restore();
    painter.setPen(Qt::blue);
    painter.setBrush(QBrush(Qt::blue));
    if (rrt->guiShortPath.size())
    {
        for (int i = 0; i < (int)rrt->guiShortPath.size() - 1; i++)
        {
            painter.drawLine(QPointF(rrt->guiShortPath[i].x, rrt->guiShortPath[i].y), QPointF(rrt->guiShortPath[i + 1].x, rrt->guiShortPath[i + 1].y));
        }
    }
}   

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawField(painter);
    drawStartPos(painter);
    drawEndPos(painter);

    drawObstacles(painter);
    drawNodes(painter);
    emit painting();
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMouseClickedPoint = event->pos();
        rrt->endPos.x()=lastMouseClickedPoint.x(); 
        rrt->endPos.y() =  lastMouseClickedPoint.y();
        scribbling = true;
        //std::cout<<rrt->endPos.x()<<"\t"<<rrt->endPos.y()<<endl;
        update();
    }
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{

}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::LeftButton && scribbling) {
    //     QPoint curPoint = event->pos();
    //     rrt->obstacles->addObstacle(Vector2f(lastMouseClickedPoint.x(), lastMouseClickedPoint.y()), Vector2f(curPoint.x(), curPoint.y()));
    //     update();
    //     scribbling = false;
    // }
}
