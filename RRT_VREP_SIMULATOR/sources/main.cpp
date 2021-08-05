#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <object.h>
#include <robot.h>
extern "C" {
#include "extApi.h"
}

int clientID;
robot Robots;
datas sts;
object obs[NUMBER_OBS];
int main(int argc, char *argv[])
{
	std::string ten = "R";
    clientID = simxStart((simxChar*)"127.0.0.1", ID, true, true, 2000, 5);
    if(clientID!= -1)
    {
        Robots.setClientID(clientID);
        Robots.getHandle();
        Robots.setVel(0,0);
        for(int i = 0;i< NUMBER_OBS;i++)
        {
        	object temp_obs(ten + to_string(i));
        	obs[i] = temp_obs;
        	obs[i].setClientID(clientID);
        	obs[i].getHandle();
        	obs[i].getData();
        }
    }
    else
    {
        std::cout << "FAIL TO CONNECT\n";
    }
    QApplication a(argc, argv);
    MainWindow w(6);
    w.show();

    return a.exec();
}
