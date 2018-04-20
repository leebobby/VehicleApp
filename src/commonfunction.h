/*!
 * @file src/commonfunction.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include <QString>
#include <QWidget>
#include <QDebug>
//for linux operation
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//for media
#include "AutPlayer.h"
#include "outputCtrl.h"
#include "mediaInfo.h"
using namespace android;

class CommonFunction
{
public:
    CommonFunction();
    bool fileExistOrNot(QString path);
    bool video_support_or_not(QWidget* parent_widget,QString path);
    QString print_info(unsigned char tmp[],int tmp2);
};

int autCb_func(int32_t msgType, void *user,void* data,int len);
#endif // COMMONFUNCTION_H
