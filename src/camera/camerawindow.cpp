/*!
 * @file camerawindow.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "camerawindow.h"
#include "ui_camerawindow.h"

#include <QPoint>
#include <QDebug>
#include <QMutex>

/*!
 * @pGlobalMainDesk
 * this global var is defined in maindesk, for widget return
 */
#include "src/maindesk.h"
extern MainDesk* pGlobalMainDesk;

CameraWindow *CameraWindow::self = NULL;
CameraWindow *CameraWindow::Instance(QWidget *parent)
{
    if (self == NULL) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self == NULL) {
            qDebug()<<"now self is null, new a instance";
            self = new CameraWindow(parent);
        }
    }
    return self;
}


CameraWindow::CameraWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWindow)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);//the ~CameraWindow will be call when func close() run

    m_pSwitchButton = new SwitchButton(this);
    m_pSwitchButton->setToggled(false);
    m_pSwitchButton->move(QPoint(930,290));
    connect(m_pSwitchButton, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));


    qDebug()<<"CameraWindow construction done";
}

CameraWindow::~CameraWindow()
{
    delete ui;
    self = NULL;
}

void CameraWindow::onToggled(bool ischecked)
{
    qDebug()<< "State : " << ischecked;
}

void CameraWindow::on_returnButton_clicked()
{
    qDebug()<<"CameraWindow::on_returnButton_clicked";
#if 0
    MainDesk* pMaindesk = static_cast<MainDesk*>(parentWidget());
    if(pMaindesk == NULL){
        qDebug()<<"Oops!! parentWidget is NULL";
        return;
    }
    pMaindesk->setHidden(false);
#endif
    pGlobalMainDesk->setHidden(false);
    this->close();
}

void CameraWindow::on_exchangeButton_clicked()
{

}

void CameraWindow::on_picFolderButton_clicked()
{

}

void CameraWindow::on_settingButton_clicked()
{
    CameraSetting::Instance()->show();
    this->setHidden(true);
}

void CameraWindow::on_lockButton_clicked()
{

}

void CameraWindow::on_recordButton_clicked()
{

}

void CameraWindow::on_takePhotoButton_clicked()
{

}
