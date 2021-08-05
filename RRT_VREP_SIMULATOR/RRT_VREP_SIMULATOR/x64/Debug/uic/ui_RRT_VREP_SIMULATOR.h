/********************************************************************************
** Form generated from reading UI file 'RRT_VREP_SIMULATOR.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RRT_VREP_SIMULATOR_H
#define UI_RRT_VREP_SIMULATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RRT_VREP_SIMULATORClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RRT_VREP_SIMULATORClass)
    {
        if (RRT_VREP_SIMULATORClass->objectName().isEmpty())
            RRT_VREP_SIMULATORClass->setObjectName(QString::fromUtf8("RRT_VREP_SIMULATORClass"));
        RRT_VREP_SIMULATORClass->resize(600, 400);
        menuBar = new QMenuBar(RRT_VREP_SIMULATORClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        RRT_VREP_SIMULATORClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RRT_VREP_SIMULATORClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        RRT_VREP_SIMULATORClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(RRT_VREP_SIMULATORClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        RRT_VREP_SIMULATORClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RRT_VREP_SIMULATORClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RRT_VREP_SIMULATORClass->setStatusBar(statusBar);

        retranslateUi(RRT_VREP_SIMULATORClass);

        QMetaObject::connectSlotsByName(RRT_VREP_SIMULATORClass);
    } // setupUi

    void retranslateUi(QMainWindow *RRT_VREP_SIMULATORClass)
    {
        RRT_VREP_SIMULATORClass->setWindowTitle(QApplication::translate("RRT_VREP_SIMULATORClass", "RRT_VREP_SIMULATOR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RRT_VREP_SIMULATORClass: public Ui_RRT_VREP_SIMULATORClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RRT_VREP_SIMULATOR_H
