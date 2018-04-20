/*!
 * @file maindesk.cpp
 * Copyright (c) 2018
 * @brief this window is main window, user see it firstly
 * detailed des
 * mainly open what you want
 * @date 2018
 * @author lee
 */
#include "maindesk.h"
#include "ui_maindesk.h"


#include <QDebug>

#ifdef LAYERCONTROL_SYSTEM
/*!
 * @function
 * @brief des
 * @args: surfaceID1 is UI's surface id; surfaceID2 is camera's surface id;
 *        rec1 is UI's positon and size; rec2 is camera's position and size;
 *        layerID is this program's layerid
 * @return
 */

int surfaceID = 5000;
int surfaceID1 = 0;
int surfaceID2 = 0;
int layerID = 4000;
int detectNum = 0;// 0:no call; 1: call
int detectType = -1;//-1:only ui; 0:camera in; 1: camera out;  2:media in;  3:media out
void setDisplayBySystem()
{
    qDebug()<<"setDisplayBySystem is begin";
    system("LayerManagerControl set layer 1000 visibility 0");
    system("LayerManagerControl set layer 4000 height 600");
    //command_0 is camera out
    QString command_0 = QString("LayerManagerControl remove surface %1 from layer %2")\
                        .arg(getpid()+18000).arg(layerID);
    //command_1&command_2 is camera or media in
    QString command_1 = QString("LayerManagerControl set layer %1 render order [%2,%3]")\
                        .arg(layerID).arg(surfaceID1).arg(surfaceID2);
    QString command_2 = QString("LayerManagerControl add surface %1 to layer %2")\
                        .arg(surfaceID1).arg(layerID);
    //commad_3 is media out
    QString command_3 = QString("LayerManagerControl remove surface %1 from layer %2")\
                        .arg(getpid()+9000).arg(layerID);
    qDebug()<<"here------";
    switch(detectType){
    case 0://camera in
        system(command_1.toLatin1().data());
        system(command_2.toLatin1().data());
        qDebug()<<"run command:";
        qDebug()<<command_1.toLatin1().data();
        qDebug()<<command_2.toLatin1().data();
        detectType = -1;
        break;
    case 1://camear out
        system(command_0.toLatin1().data());
        qDebug()<<"run command:";
        qDebug()<<command_0.toLatin1().data();
        detectType = -1;
        break;
    case 2://media in
        system(command_1.toLatin1().data());
        system(command_2.toLatin1().data());
        detectType = -1;
        qDebug()<<"run command:";
        qDebug()<<command_1.toLatin1().data();
        qDebug()<<command_2.toLatin1().data();
        detectType = -1;
        break;
    case 3://media out
        qDebug()<<"now deal media out signal";
#if 1
        system(command_0.toLatin1().data());
        qDebug()<<"run command:";
        qDebug()<<command_0.toLatin1().data();
#endif
        detectType = -1;
        break;
    default:
        qDebug()<<"unknow operation";
        break;
    }
    qDebug()<<"void setDisplayBySystem() is over";
}

DetectThread::DetectThread(QObject *parent)
    : QThread(parent)
{
    qDebug() << "DetectThread Thread : " << QThread::currentThreadId();
}
DetectThread::~DetectThread()
{
    qDebug()<<"~DetectThread";
}

void DetectThread::DetectThread::run()
{
    qDebug() << "DetectThread Run Thread : " << QThread::currentThreadId() ;
#if 1
    while(1){
        msleep(100);
        if(detectNum == 1){
            qDebug()<<"DetectThread run setDisplayBySystem";
            setDisplayBySystem();
            qDebug()<<"DetectThread complete";
            detectNum = 0;
        }else{
            //qDebug()<<"no need to run setDisplayBySystem";
        }
    }//while
#endif
}
#endif

/*!
 * @pGlobalMainDesk
 * this is a temp global var for clildren widget return!!!
 * when window manager is ok this global should be delete!!!
 */
MainDesk* pGlobalMainDesk = NULL;


MainDesk::MainDesk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainDesk)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);

#ifdef LAYERCONTROL_SYSTEM
    detecting = new DetectThread(this);
    detecting->start();
#endif

    pGlobalMainDesk = this;
}

MainDesk::~MainDesk()
{
    delete ui;
}
/*!
 * @function:on_DvrButton_clicked
 * @brief des this is a slot which will open Dvr
 */
void MainDesk::on_DvrButton_clicked()
{
    qDebug()<<"on_DvrButton_clicked";
    this->hide();
    CameraWindow::Instance()->show();


#ifdef LAYERCONTROL_SYSTEM
    surfaceID1 = ++surfaceID;
    surfaceID2 = getpid() + 18000;
    qDebug()<<" MainDesk::on_DvrButton_clicked surfaceID increase 1"<<surfaceID;
    detectNum = 1;
    detectType = 1;
#endif
}
/*!
 * @function:on_musicButton_clicked
 * @brief des this is a slot which will open musicPlayer
 */
void MainDesk::on_musicButton_clicked()
{
    qDebug()<<"on_musicButton_clicked";
    MusicPlayer::Instance()->show();
    this->setHidden(true);
}
/*!
 * @function:on_videoButton_clicked
 * @brief des this is a slot which will open videoPlayer
 */
void MainDesk::on_videoButton_clicked()
{
    qDebug()<<"on_videoButton_clicked";
}
/*!
 * @function:on_carLifeButton_clicked
 * @brief des this is a slot which will interconnect to phone with wifi and adb
 */
void MainDesk::on_carLifeButton_clicked()
{
    qDebug()<<"on_carLifeButton_clicked";
}
/*!
 * @function:on_guidanceButton_clicked
 * @brief des this is a slot which will open compass
 */
void MainDesk::on_guidanceButton_clicked()
{
    qDebug()<<"on_guidanceButton_clicked";
    Gallery::Instance()->show();
    Gallery::Instance()->initImageList();
    this->setHidden(true);
}
