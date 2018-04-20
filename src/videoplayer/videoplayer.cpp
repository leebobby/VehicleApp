/*!
 * @file videoplayer.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "videoplayer.h"
#include "ui_videoplayer.h"

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

/*!
 * @pGlobalMainDesk
 * this global var is defined in maindesk, for widget return
 */
#include "src/maindesk.h"
extern MainDesk* pGlobalMainDesk;

VideoPlayer *VideoPlayer::self = NULL;
VideoPlayer *VideoPlayer::Instance(QWidget *parent)
{
    if (self == NULL) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self == NULL) {
            self = new VideoPlayer(parent);
        }
    }
    return self;
}


VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoPlayer)
{
    ui->setupUi(this);
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
    self = NULL;
}

void VideoPlayer::on_homeButton_clicked()
{
    pGlobalMainDesk->setHidden(false);
    this->close();
#if 0//this is a bug
    MainDesk* pMaindesk = static_cast<MainDesk*>(parentWidget());
    if(pMaindesk == NULL){
        qDebug()<<"Oops!! parentWidget is NULL";
        return;
    }
    pMaindesk->setHidden(false);
    this->close();
#endif
}
