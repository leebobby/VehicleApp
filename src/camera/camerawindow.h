/*!
 * @file camerawindow.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


#include <QWidget>

#include "src/switchbutton.h"
#include "camerasetting.h"
namespace Ui {
class CameraWindow;
}

class CameraWindow : public QWidget
{
    Q_OBJECT

public:
    ~CameraWindow();
    static CameraWindow *Instance(QWidget *parent = 0);//create window by this interface
private slots:
    void onToggled(bool);//for switch button;

    void on_returnButton_clicked();

    void on_exchangeButton_clicked();

    void on_picFolderButton_clicked();

    void on_settingButton_clicked();

    void on_lockButton_clicked();

    void on_recordButton_clicked();

    void on_takePhotoButton_clicked();

private:
    static CameraWindow* self;//this  is for single instance
    Ui::CameraWindow *ui;
    SwitchButton* m_pSwitchButton;//for camera preview on or off
    explicit CameraWindow(QWidget *parent = 0);
};

#endif // CAMERAWINDOW_H
