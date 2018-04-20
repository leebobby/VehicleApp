/*!
 * @file maindesk.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef MAINDESK_H
#define MAINDESK_H

#include "camera/camerawindow.h"
#include "musicplayer/musicplayer.h"
#include "gallery/gallery.h"
#include "videoplayer/videoplayer.h"

#include <QWidget>
#include <QThread>
namespace Ui {
class MainDesk;
}
#ifdef LAYERCONTROL_SYSTEM
/*!
 * @DetectThread
 * @brief: Detect setDisplayBySystem should run or not
 */
class DetectThread : public QThread
{
    Q_OBJECT

public:
    explicit DetectThread(QObject *parent = 0);
    ~DetectThread();
protected:
    virtual void run();
signals:

private:

};
#endif
class MainDesk : public QWidget
{
    Q_OBJECT

public:
    explicit MainDesk(QWidget *parent = 0);
    ~MainDesk();

private slots:
    void on_DvrButton_clicked();

    void on_musicButton_clicked();

    void on_videoButton_clicked();

    void on_carLifeButton_clicked();

    void on_guidanceButton_clicked();

private:
    Ui::MainDesk *ui;

#ifdef LAYERCONTROL_SYSTEM
    DetectThread* detecting;
#endif
};

#endif // MAINDESK_H
