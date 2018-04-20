/*!
 * @file musicplayer.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QPixmap>
#include "playlist.h"
#include "../commonfunction.h"
#include "xmetadataretriever.h"

enum PLAYSTATE{
    PLAYING = 1,
    PAUSEED = 2,
    STOPPED = 3,
    PREPARED = 4,
    PREPARING = 5,
    SEEKING = 6
};

extern int autCb_func(int32_t msgType, void *user,void* data,int len);

class MusicInfoThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicInfoThread(QString path,QObject *parent = 0);
    ~MusicInfoThread();
protected:
    virtual void run();
signals:
    void complete(QString,QString,QString,QPixmap);
private:
    QString m_filepath;
    QString m_albumName;
    QString m_musicName;
    QString m_singerName;
    QPixmap m_poster;
    CommonFunction* commonfunc;
};


namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    static MusicPlayer *Instance(QWidget *parent = 0);//create window by this interface
    ~MusicPlayer();
    void initAutPlayer();
    void startPlay();
    void updateDurationInfo(qint64 currentInfo);
    void getMusicInfo(QString path);


private slots:
    void on_preMusicButton_clicked();

    void on_playButton_clicked();

    void on_nextMusicButton_clicked();

    void on_searchButton_clicked();

    void on_playListButton_clicked();

    void on_playOrderButton_clicked();

    void on_setttingButton_clicked();

    void on_pushButton_clicked();

    void on_notifyMusicPlayerToPlay(QList<QFileInfo>,int);

    void on_notifyMusicPlayerListChanged(QList<QFileInfo>);

    void timerUpdate();

    void onProgressSliderMoved(int);

    void onProgressSliderReleased();

    void setMusicInfo(QString,QString,QString,QPixmap);

signals:
    void hidePlayList();

    void showPlayList();

protected:
    void focusInEvent(QFocusEvent *e);
private:
    Ui::MusicPlayer *ui;

    static MusicPlayer* self;//this  is for single instance
    explicit MusicPlayer(QWidget *parent = 0);

    CommonFunction* m_commonFunction;
    PlayList *m_pPlayList;


    PLAYSTATE m_bPlayState;//ENUM of state
    int m_bMusicListOpen;//for open is 1;close is 0
    QList<QFileInfo> m_playList;
    QString m_curMusicFilePath;
    int m_curMusicFileCount;

    AUTPlayer* ap;
    qint64 m_totalDuration;
    qint64 m_curDuration;
    QTimer* progressSliderTimer;

    QString m_albumName;
    QString m_musicName;
    QString m_singerName;
    QPixmap m_poster;
};

#endif // MUSICPLAYER_H
