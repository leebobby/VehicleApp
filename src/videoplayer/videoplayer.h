/*!
 * @file videoplayer.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    static VideoPlayer *Instance(QWidget *parent = 0);//create window by this interface

    ~VideoPlayer();

private slots:
    void on_homeButton_clicked();

private:
    Ui::VideoPlayer *ui;
    explicit VideoPlayer(QWidget *parent = 0);
    static VideoPlayer* self;//this  is for single instance
};

#endif // VIDEOPLAYER_H
